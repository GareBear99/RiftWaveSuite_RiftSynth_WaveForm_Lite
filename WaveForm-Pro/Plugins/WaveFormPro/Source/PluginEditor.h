#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../../Shared/Core/ParameterBinding.h"
#include "../../../Shared/UI/NeonLookAndFeel.h"
#include "../../../Shared/UI/SceneRenderer.h"
#include "../../../Shared/UI/PresetBrowserComponent.h"
#include "../../../Shared/UI/ThemeBank.h"

class WaveFormProEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit WaveFormProEditor(WaveFormProProcessor&);
    void resized() override;
    void paint(juce::Graphics&) override;

private:
    void timerCallback() override;
    void refreshPresetList();
    WaveFormProProcessor& processor;
    riftwave::NeonLookAndFeel lnf;
    riftwave::SceneMode mode = riftwave::SceneMode::Ribbon;
    riftwave::SceneRenderer scene;
    riftwave::PresetBrowserComponent presetBrowser;
    juce::TextEditor debugPanel;

    juce::Slider gain, mix, smooth, density, width, peakHold;
    juce::ComboBox sceneBox;
    juce::ToggleButton beautyMode { "Beauty" }, uiSfx { "UI SFX" };

    riftwave::SliderBinding gainA, mixA, smoothA, densityA, widthA, peakHoldA;
    riftwave::ComboBinding sceneA;
    riftwave::ButtonBinding beautyA, uiSfxA;
};
