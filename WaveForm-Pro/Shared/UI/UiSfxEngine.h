#pragma once
#include <JuceHeader.h>

namespace riftwave {
class UiSfxEngine
{
public:
    enum class Cue { Click, Spark, Bloom, Rise, Tick };

    void setEnabled(bool enabled) noexcept { isEnabled = enabled; }
    bool getEnabled() const noexcept { return isEnabled; }

    juce::StringArray getRegisteredCues() const
    {
        return {"click", "spark", "bloom", "rise", "tick"};
    }

    juce::String cueName(Cue cue) const
    {
        switch (cue)
        {
            case Cue::Click: return "click";
            case Cue::Spark: return "spark";
            case Cue::Bloom: return "bloom";
            case Cue::Rise:  return "rise";
            case Cue::Tick:  return "tick";
        }
        return "click";
    }

    void trigger(Cue cue) noexcept
    {
        if (! isEnabled)
            return;
        lastCue = cueName(cue);
        ++triggerCount;
        lastTriggerMs = juce::Time::getMillisecondCounterHiRes();
    }

    juce::String getLastCueName() const noexcept { return lastCue; }
    int getTriggerCount() const noexcept { return triggerCount; }
    double getLastTriggerMs() const noexcept { return lastTriggerMs; }

private:
    bool isEnabled = true;
    juce::String lastCue = "click";
    int triggerCount = 0;
    double lastTriggerMs = 0.0;
};
}
