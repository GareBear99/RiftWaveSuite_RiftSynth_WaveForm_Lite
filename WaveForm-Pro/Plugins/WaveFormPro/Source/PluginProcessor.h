#pragma once
#include <JuceHeader.h>
#include "../../../Shared/Analysis/AnalyzerState.h"
#include "../../../Shared/Core/SharedParameters.h"
#include "../../../Shared/Core/TransportState.h"
#include "../../../Shared/Core/SessionDebugSnapshot.h"
#include "../../../Shared/Core/ParameterSmootherBank.h"
#include "../../../Shared/UI/UiSfxEngine.h"
#include "../../../Shared/UI/PerformanceModeController.h"
#include "../../../Shared/Export/WavPreviewExporter.h"

class WaveFormProProcessor : public juce::AudioProcessor {
public:
    WaveFormProProcessor();
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "WaveForm Pro"; }
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

    bool saveUserPreset(const juce::String& presetName);
    bool loadUserPreset(const juce::String& presetName);
    juce::StringArray getUserPresetNames() const;
    bool exportWavPreviewFile(juce::File& outFile);
    riftwave::SessionDebugSnapshot getDebugSnapshot() const;

    riftwave::AnalyzerState analyzer;
    riftwave::TransportState transport;
    riftwave::UiSfxEngine uiSfx;
    juce::AudioProcessorValueTreeState apvts;

private:
    riftwave::ParameterSmootherBank smoothers;
    riftwave::PerformanceModeController performanceMode;
};
