#include "AnalyzerState.h"

namespace riftwave {
AnalyzerState::AnalyzerState() = default;

void AnalyzerState::prepare(double newSampleRate, int)
{
    const juce::ScopedLock sl(lock);
    sampleRate = newSampleRate;
    fifo.fill(0.0f);
    fftData.fill(0.0f);
    snapshot = {};
    previousEnergy = 0.0f;
    peakHoldValue = 0.0f;
    peakHoldCounter = 0;
    fifoIndex = 0;
}

void AnalyzerState::push(const juce::AudioBuffer<float>& buffer)
{
    const auto channels = juce::jmin(2, buffer.getNumChannels());
    if (channels <= 0) return;

    const auto numSamples = buffer.getNumSamples();
    const auto* left = buffer.getReadPointer(0);
    const auto* right = channels > 1 ? buffer.getReadPointer(1) : left;

    AnalyzerSnapshot local = {};
    local.peakL = buffer.getMagnitude(0, 0, numSamples);
    local.rmsL = buffer.getRMSLevel(0, 0, numSamples);
    local.peakR = channels > 1 ? buffer.getMagnitude(1, 0, numSamples) : local.peakL;
    local.rmsR = channels > 1 ? buffer.getRMSLevel(1, 0, numSamples) : local.rmsL;

    float sumMul = 0.0f, sumL2 = 0.0f, sumR2 = 0.0f, energy = 0.0f;
    for (int i = 0; i < numSamples; ++i)
    {
        const auto l = left[i];
        const auto r = right[i];
        const auto mono = 0.5f * (l + r);
        pushSample(mono);
        sumMul += l * r;
        sumL2 += l * l;
        sumR2 += r * r;
        energy += std::abs(mono);
    }

    local.stereoWidth = juce::jlimit(0.0f, 1.0f,
        std::abs(local.rmsL - local.rmsR) +
        (1.0f - std::abs(sumMul / (std::sqrt(sumL2 * sumR2) + 1.0e-6f))));
    local.correlation = juce::jlimit(-1.0f, 1.0f, sumMul / (std::sqrt(sumL2 * sumR2) + 1.0e-6f));
    const auto normalizedEnergy = energy / juce::jmax(1, numSamples);
    local.transient = juce::jlimit(0.0f, 1.0f, juce::jmax(0.0f, normalizedEnergy - previousEnergy) * 4.5f);
    previousEnergy = normalizedEnergy;

    const auto rawPeak = juce::jmax(local.peakL, local.peakR);
    if (rawPeak >= peakHoldValue)
    {
        peakHoldValue = rawPeak;
        peakHoldCounter = (int) std::round(peakHoldSeconds * sampleRate / (double) kFftSize);
    }
    else if (--peakHoldCounter <= 0)
    {
        peakHoldValue = juce::jmax(rawPeak, peakHoldValue * (beautyMode ? 0.985f : 0.96f));
    }
    local.peakHold = peakHoldValue;

    {
        const juce::ScopedLock sl(lock);
        local.spectrum = snapshot.spectrum;
        const auto smooth = beautyMode ? smoothing : juce::jmin(0.70f, smoothing);
        snapshot.peakL = juce::jmax(local.peakL, snapshot.peakL * smooth);
        snapshot.peakR = juce::jmax(local.peakR, snapshot.peakR * smooth);
        snapshot.rmsL = local.rmsL + (snapshot.rmsL - local.rmsL) * smooth;
        snapshot.rmsR = local.rmsR + (snapshot.rmsR - local.rmsR) * smooth;
        snapshot.stereoWidth = local.stereoWidth + (snapshot.stereoWidth - local.stereoWidth) * smooth;
        snapshot.correlation = local.correlation + (snapshot.correlation - local.correlation) * smooth;
        snapshot.transient = local.transient + (snapshot.transient - local.transient) * 0.4f;
        snapshot.peakHold = local.peakHold;
    }
}

AnalyzerSnapshot AnalyzerState::getSnapshot() const
{
    const juce::ScopedLock sl(lock);
    return snapshot;
}

void AnalyzerState::pushSample(float mono)
{
    fifo[(size_t) fifoIndex++] = mono;
    if (fifoIndex == kFftSize)
    {
        computeFrame();
        fifoIndex = 0;
    }
}

void AnalyzerState::computeFrame()
{
    std::copy(fifo.begin(), fifo.end(), fftData.begin());
    std::fill(fftData.begin() + kFftSize, fftData.end(), 0.0f);
    window.multiplyWithWindowingTable(fftData.data(), kFftSize);
    fft.performFrequencyOnlyForwardTransform(fftData.data());

    const juce::ScopedLock sl(lock);
    const auto localSmooth = beautyMode ? smoothing : juce::jmin(0.72f, smoothing);
    for (int i = 0; i < kVisualBands; ++i)
        snapshot.spectrum[(size_t) i] *= localSmooth;

    for (int bin = 1; bin < kFftSize / 2; ++bin)
    {
        const auto mag = juce::Decibels::gainToDecibels(fftData[(size_t) bin] / (float) kFftSize, -100.0f);
        const auto norm = juce::jlimit(0.0f, 1.0f, juce::jmap(mag, -72.0f, 0.0f, 0.0f, 1.0f));
        const auto band = mapBinToBand(bin);
        snapshot.spectrum[(size_t) band] = juce::jmax(snapshot.spectrum[(size_t) band], norm);
    }

    snapshot.lowEnergy = bandEnergyForHzRange(20.0f, 180.0f);
    snapshot.midEnergy = bandEnergyForHzRange(180.0f, 2500.0f);
    snapshot.highEnergy = bandEnergyForHzRange(2500.0f, 12000.0f);
}

float AnalyzerState::bandEnergyForHzRange(float minHz, float maxHz) const noexcept
{
    float sum = 0.0f;
    int count = 0;
    for (int bin = 1; bin < kFftSize / 2; ++bin)
    {
        const auto frequency = (float) bin * (float) sampleRate / (float) kFftSize;
        if (frequency < minHz || frequency >= maxHz)
            continue;
        const auto band = mapBinToBand(bin);
        sum += snapshot.spectrum[(size_t) band];
        ++count;
    }
    return count > 0 ? sum / (float) count : 0.0f;
}

int AnalyzerState::mapBinToBand(int fftBin) const noexcept
{
    const auto frequency = (float) fftBin * (float) sampleRate / (float) kFftSize;
    const auto logMin = std::log10(20.0f);
    const auto logMax = std::log10(20000.0f);
    const auto logFreq = std::log10(juce::jlimit(20.0f, 20000.0f, frequency));
    const auto t = (logFreq - logMin) / (logMax - logMin);
    return juce::jlimit(0, kVisualBands - 1, (int) std::floor(t * (float) (kVisualBands - 1)));
}
}
