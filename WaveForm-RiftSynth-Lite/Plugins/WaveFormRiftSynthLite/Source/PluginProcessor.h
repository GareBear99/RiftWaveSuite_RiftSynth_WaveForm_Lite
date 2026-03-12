#pragma once
#include <JuceHeader.h>
#include "../../../Shared/Analysis/AnalyzerState.h"
#include "../../../Shared/Core/SharedParameters.h"
#include "../../../Shared/Core/TransportState.h"
#include "../../../Shared/Core/StepScheduler.h"
#include "../../../Shared/Core/SessionDebugSnapshot.h"
#include "../../../Shared/Seed/SeedEngine.h"
#include "../../../Shared/Synth/SimpleVoice.h"
#include "../../../Shared/Synth/OversamplingEngine.h"
#include "../../../Shared/Playback/PatternPlayer.h"
#include "../../../Shared/UI/UiSfxEngine.h"

class WaveFormRiftSynthLiteProcessor : public juce::AudioProcessor {
public:
    WaveFormRiftSynthLiteProcessor();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "WaveForm / RiftSynth Lite"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    void rebuildPattern();
    juce::String getSeedCode() const;
    riftwave::SessionDebugSnapshot getDebugSnapshot() const;

    riftwave::AnalyzerState analyzer;
    riftwave::TransportState transport;
    riftwave::StepScheduler scheduler;
    riftwave::SeedEngine seed;
    riftwave::VoiceManager voices;
    riftwave::PatternPlayer player;
    riftwave::UiSfxEngine uiSfx;
    juce::AudioProcessorValueTreeState apvts;
private:
    std::vector<riftwave::LaneEvent> events;
    riftwave::OversamplingEngine oversampling;
};
