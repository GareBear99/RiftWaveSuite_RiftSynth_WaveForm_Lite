#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct SessionDebugSnapshot
{
    double bpm = 120.0;
    bool hostPlaying = false;
    int currentStep = 0;
    float peak = 0.0f;
    float rms = 0.0f;
    juce::String seedCode;
    juce::String lastUiCue;
};
}
