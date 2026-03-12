#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct CompileReadiness
{
    static constexpr const char* suiteVersion = "DARPA_v8";
    static constexpr const char* juceFloor = "JUCE 8.x expected";
    static constexpr const char* cmakeFloor = "CMake 3.22+ expected";

    static juce::StringArray requiredFormatsForMac() { return { "AU", "VST3" }; }
    static juce::StringArray requiredFormatsForWindows() { return { "VST3" }; }
    static juce::StringArray requiredHostMatrix() { return { "Logic Pro", "Ableton Live", "Reaper", "FL Studio" }; }
};
}
