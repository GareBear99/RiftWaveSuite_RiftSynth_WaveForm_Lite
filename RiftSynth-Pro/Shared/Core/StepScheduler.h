#pragma once
#include <JuceHeader.h>
#include "TransportState.h"

namespace riftwave {
struct StepClockSnapshot
{
    int currentStep = 0;
    int stepsPerBar = 16;
    int samplesUntilNextStep = 0;
    double bpm = 120.0;
    bool hostSynced = true;
    bool barJustWrapped = false;
};

class StepScheduler
{
public:
    void prepare(double newSampleRate, int newStepsPerBar = 16)
    {
        sampleRate = newSampleRate > 1.0 ? newSampleRate : 44100.0;
        state.stepsPerBar = juce::jmax(1, newStepsPerBar);
        state.samplesUntilNextStep = 0;
        state.currentStep = 0;
        state.barJustWrapped = false;
        wasPlaying = false;
    }

    void resetToBarStart() noexcept
    {
        state.currentStep = 0;
        state.samplesUntilNextStep = 0;
        state.barJustWrapped = true;
    }

    void setTransportBpm(double newBpm) noexcept
    {
        state.bpm = juce::jlimit(20.0, 300.0, newBpm);
    }

    void setHostSynced(bool shouldSync) noexcept { state.hostSynced = shouldSync; }

    void syncToTransport(const TransportSnapshot& transport) noexcept
    {
        setTransportBpm(transport.bpm > 1.0 ? transport.bpm : 120.0);
        if (! state.hostSynced)
            return;

        if (transport.isPlaying && (! wasPlaying || transport.ppq < lastPpq))
        {
            const auto stepFloat = transport.ppq * 4.0;
            const auto floorStep = (int) std::floor(stepFloat);
            const auto frac = juce::jlimit(0.0, 0.999999, stepFloat - (double) floorStep);
            state.currentStep = ((floorStep % state.stepsPerBar) + state.stepsPerBar) % state.stepsPerBar;
            state.samplesUntilNextStep = (int) juce::jlimit(0.0,
                                                            (double) juce::jmax(1, getSamplesPerStep()),
                                                            (1.0 - frac) * (double) getSamplesPerStep());
            if (frac < 1.0e-6)
                state.samplesUntilNextStep = 0;
            state.barJustWrapped = (state.currentStep == 0);
        }

        wasPlaying = transport.isPlaying;
        lastPpq = transport.ppq;
    }

    template <typename Fn>
    void processBlock(int numSamples, Fn&& onStep)
    {
        state.barJustWrapped = false;
        auto remaining = numSamples;
        auto offset = 0;

        while (remaining > 0)
        {
            if (state.samplesUntilNextStep <= 0)
            {
                const bool isBarStart = (state.currentStep == 0);
                state.barJustWrapped = isBarStart;
                onStep(state.currentStep, offset, isBarStart);
                state.currentStep = (state.currentStep + 1) % state.stepsPerBar;
                state.samplesUntilNextStep = getSamplesPerStep();
            }

            const auto chunk = juce::jmin(remaining, state.samplesUntilNextStep);
            remaining -= chunk;
            offset += chunk;
            state.samplesUntilNextStep -= chunk;
        }
    }

    int getSamplesPerStep() const noexcept
    {
        return (int) juce::jmax(1.0, sampleRate * 60.0 / juce::jmax(1.0, state.bpm) / 4.0);
    }

    const StepClockSnapshot& getSnapshot() const noexcept { return state; }

private:
    double sampleRate = 44100.0;
    double lastPpq = 0.0;
    bool wasPlaying = false;
    StepClockSnapshot state;
};
}
