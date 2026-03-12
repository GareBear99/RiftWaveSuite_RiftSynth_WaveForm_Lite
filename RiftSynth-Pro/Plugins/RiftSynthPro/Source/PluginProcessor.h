#pragma once
#include <JuceHeader.h>
#include "../../../Shared/Core/SharedParameters.h"
#include "../../../Shared/Core/TransportState.h"
#include "../../../Shared/Core/StepScheduler.h"
#include "../../../Shared/Core/SessionDebugSnapshot.h"
#include "../../../Shared/Core/ParameterSmootherBank.h"
#include "../../../Shared/Seed/SeedEngine.h"
#include "../../../Shared/Synth/SimpleVoice.h"
#include "../../../Shared/Synth/OversamplingEngine.h"
#include "../../../Shared/Playback/PatternPlayer.h"
#include "../../../Shared/UI/UiSfxEngine.h"
#include "../../../Shared/Export/WavPreviewExporter.h"

class RiftSynthProProcessor : public juce::AudioProcessor {
public:
    RiftSynthProProcessor();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "RiftSynth Pro"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.1; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::String getSeedCode() const;
    void rebuildPattern();
    juce::StringArray buildVariationPreview() const;
    juce::File exportMidiPreviewFile() const;
    bool exportWavPreviewFile(juce::File& outFile);
    bool saveUserPreset(const juce::String& presetName);
    bool loadUserPreset(const juce::String& presetName);
    juce::StringArray getUserPresetNames() const;
    riftwave::SessionDebugSnapshot getDebugSnapshot() const;

    riftwave::TransportState transport;
    riftwave::StepScheduler scheduler;
    riftwave::SeedEngine seed;
    riftwave::VoiceManager voices;
    riftwave::UiSfxEngine uiSfx;
    juce::AudioProcessorValueTreeState apvts;

private:
    void updateVoiceTargets();

    double sr = 44100.0;
    juce::MidiMessageSequence exportedPreview;
    int currentStep = 0;
    bool wasPlaying = false;
    riftwave::PatternPlayer patternPlayer;
    std::vector<riftwave::LaneEvent> events;
    riftwave::OversamplingEngine oversampling;
    riftwave::ParameterSmootherBank smoothers;
};
