#pragma once
#include <JuceHeader.h>

namespace riftwave {

class ParameterSmootherBank
{
public:
    void prepare (double sampleRate)
    {
        sr = sampleRate;
        for (auto& [_, smoother] : smoothers)
            smoother.reset (sr, 0.03);
    }

    void setCurrentAndTarget (const juce::String& id, float value)
    {
        auto& smoother = smoothers[id];
        smoother.reset (sr, 0.03);
        smoother.setCurrentAndTargetValue (value);
    }

    void setTarget (const juce::String& id, float value)
    {
        auto& smoother = smoothers[id];
        smoother.reset (sr, 0.03);
        smoother.setTargetValue (value);
    }

    float getNextValue (const juce::String& id)
    {
        auto& smoother = smoothers[id];
        return smoother.getNextValue();
    }

private:
    double sr = 44100.0;
    std::map<juce::String, juce::SmoothedValue<float>> smoothers;
};

} // namespace riftwave
