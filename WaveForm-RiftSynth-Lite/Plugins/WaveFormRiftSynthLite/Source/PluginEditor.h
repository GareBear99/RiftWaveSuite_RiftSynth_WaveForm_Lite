#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../../Shared/Core/ParameterBinding.h"
#include "../../../Shared/UI/NeonLookAndFeel.h"
#include "../../../Shared/UI/AnalyzerComponent.h"
#include "../../../Shared/UI/SeedStripComponent.h"

class WaveFormRiftSynthLiteEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit WaveFormRiftSynthLiteEditor(WaveFormRiftSynthLiteProcessor&);
    void resized() override;
    void paint(juce::Graphics&) override;

private:
    void timerCallback() override;
    WaveFormRiftSynthLiteProcessor& processor;
    riftwave::NeonLookAndFeel lnf;
    riftwave::AnalyzerComponent analyzer;
    riftwave::SeedStripComponent seedStrip;
    juce::TextEditor debugPanel;
    juce::Slider seed, gain;
    juce::ToggleButton uiSfx { "UI SFX" };
    riftwave::SliderBinding seedA, gainA;
    riftwave::ButtonBinding uiSfxA;
};
