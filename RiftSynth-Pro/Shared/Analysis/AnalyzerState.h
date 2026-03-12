#pragma once
#include <JuceHeader.h>
#include <array>

namespace riftwave {
constexpr int kFftOrder = 11;
constexpr int kFftSize = 1 << kFftOrder;
constexpr int kVisualBands = 128;

struct AnalyzerSnapshot {
    float peakL = 0.0f;
    float peakR = 0.0f;
    float rmsL = 0.0f;
    float rmsR = 0.0f;
    float stereoWidth = 0.0f;
    float correlation = 0.0f;
    float transient = 0.0f;
    float peakHold = 0.0f;
    float lowEnergy = 0.0f;
    float midEnergy = 0.0f;
    float highEnergy = 0.0f;
    std::array<float, kVisualBands> spectrum {};
};

class AnalyzerState {
public:
    AnalyzerState();
    void prepare(double newSampleRate, int maximumBlockSize);
    void push(const juce::AudioBuffer<float>& buffer);
    AnalyzerSnapshot getSnapshot() const;
    void setSmoothing(float value) noexcept { smoothing = juce::jlimit(0.0f, 0.995f, value); }
    void setPeakHoldSeconds(float seconds) noexcept { peakHoldSeconds = juce::jlimit(0.05f, 2.5f, seconds); }
    void setBeautyMode(bool enabled) noexcept { beautyMode = enabled; }

private:
    void pushSample(float mono);
    void computeFrame();
    int mapBinToBand(int fftBin) const noexcept;
    float bandEnergyForHzRange(float minHz, float maxHz) const noexcept;

    double sampleRate = 44100.0;
    float smoothing = 0.82f;
    float peakHoldSeconds = 0.35f;
    bool beautyMode = true;
    int fifoIndex = 0;
    std::array<float, kFftSize> fifo {};
    std::array<float, 2 * kFftSize> fftData {};
    juce::dsp::FFT fft { kFftOrder };
    juce::dsp::WindowingFunction<float> window { kFftSize, juce::dsp::WindowingFunction<float>::hann };
    mutable juce::CriticalSection lock;
    AnalyzerSnapshot snapshot;
    float previousEnergy = 0.0f;
    float peakHoldValue = 0.0f;
    int peakHoldCounter = 0;
};
}
