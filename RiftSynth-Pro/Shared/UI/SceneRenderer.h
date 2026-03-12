#pragma once
#include <JuceHeader.h>
#include "../Analysis/AnalyzerState.h"
#include "../Core/ProductTypes.h"

namespace riftwave {
class SceneRenderer : public juce::Component, private juce::Timer {
public:
    SceneRenderer(AnalyzerState& analyzerRef, SceneMode& modeRef) : analyzer(analyzerRef), mode(modeRef) { startTimerHz(60); }
    void paint(juce::Graphics& g) override;
private:
    void timerCallback() override { repaint(); }
    AnalyzerState& analyzer;
    SceneMode& mode;
};
}
