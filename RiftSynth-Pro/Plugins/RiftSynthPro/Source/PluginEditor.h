#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../../Shared/Core/ParameterBinding.h"
#include "../../../Shared/UI/NeonLookAndFeel.h"
#include "../../../Shared/UI/PresetBrowserComponent.h"
#include "../../../Shared/UI/SeedStripComponent.h"
#include "../../../Shared/Export/MidiDragSource.h"

class RiftSynthProEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit RiftSynthProEditor(RiftSynthProProcessor&);
    void resized() override;
    void paint(juce::Graphics&) override;
private:
    void timerCallback() override;
    void refreshPresetList();
    RiftSynthProProcessor& processor;
    riftwave::NeonLookAndFeel lnf;
    riftwave::SeedStripComponent seedStrip;
    riftwave::PresetBrowserComponent presetBrowser;
    riftwave::MidiDragSource midiDrag;
    juce::TextEditor variations, debugPanel;
    juce::Slider seed, lanes, cutoff, gain, mutateAmount;
    juce::ComboBox wave, oversampling;
    juce::ToggleButton hostSync { "Host Sync" }, lockBass { "Lock Bass" }, lockHarmony { "Lock Harmony" }, uiSfx { "UI SFX" };

    riftwave::SliderBinding seedA, lanesA, cutoffA, gainA, mutateA;
    riftwave::ComboBinding waveA, oversamplingA;
    riftwave::ButtonBinding hostSyncA, lockBassA, lockHarmonyA, uiSfxA;
};
