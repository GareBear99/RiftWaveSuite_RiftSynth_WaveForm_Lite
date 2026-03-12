#pragma once
#include <JuceHeader.h>
#include "../Analysis/AnalyzerState.h"

namespace riftwave {
class AnalyzerComponent : public juce::Component, private juce::Timer {
public:
    explicit AnalyzerComponent(AnalyzerState& s) : state(s) { startTimerHz(30); }
    void paint(juce::Graphics& g) override;
private:
    void timerCallback() override { repaint(); }
    AnalyzerState& state;
};
}
