#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../../Shared/Presets/UserPresetStore.h"

WaveFormProProcessor::WaveFormProProcessor()
: juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
  apvts(*this, nullptr, "STATE", riftwave::SharedParameters::createLayout(riftwave::ProductTier::WaveFormPro))
{}

void WaveFormProProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    analyzer.prepare(sampleRate, samplesPerBlock);
    smoothers.prepare(sampleRate);
    smoothers.setCurrentAndTarget("gain", apvts.getRawParameterValue("gain")->load());
    smoothers.setCurrentAndTarget("mix", apvts.getRawParameterValue("mix")->load());
}

bool WaveFormProProcessor::saveUserPreset(const juce::String& presetName)
{
    uiSfx.trigger(riftwave::UiSfxEngine::Cue::Bloom);
    return riftwave::UserPresetStore::savePreset("WaveFormPro", presetName, apvts);
}

bool WaveFormProProcessor::loadUserPreset(const juce::String& presetName)
{
    const bool ok = riftwave::UserPresetStore::loadPreset("WaveFormPro", presetName, apvts);
    if (ok)
        uiSfx.trigger(riftwave::UiSfxEngine::Cue::Click);
    return ok;
}

juce::StringArray WaveFormProProcessor::getUserPresetNames() const
{
    return riftwave::UserPresetStore::listPresetNames("WaveFormPro");
}

bool WaveFormProProcessor::exportWavPreviewFile(juce::File& outFile)
{
    juce::AudioBuffer<float> temp(2, 2048);
    temp.clear();
    return riftwave::WavPreviewExporter::writeTempPreview("WaveFormPro", temp, 48000.0, outFile);
}

riftwave::SessionDebugSnapshot WaveFormProProcessor::getDebugSnapshot() const
{
    riftwave::SessionDebugSnapshot s;
    const auto snap = analyzer.getSnapshot();
    s.bpm = transport.get().bpm;
    s.hostPlaying = transport.get().isPlaying;
    s.peak = snap.peak;
    s.rms = snap.rms;
    s.lastUiCue = uiSfx.getLastCueName();
    return s;
}

void WaveFormProProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    transport.updateFrom(getPlayHead());

    smoothers.setTarget("gain", apvts.getRawParameterValue("gain")->load());
    smoothers.setTarget("mix", apvts.getRawParameterValue("mix")->load());
    const auto smoothing = apvts.getRawParameterValue("smooth")->load();
    const auto peakHold = apvts.getRawParameterValue("peakHold")->load();
    const auto beauty = apvts.getRawParameterValue("beautyMode")->load() > 0.5f;

    analyzer.setSmoothing(smoothing);
    analyzer.setPeakHoldSeconds(peakHold);
    analyzer.setBeautyMode(beauty);
    performanceMode.setPerformanceMode(! beauty);

    juce::AudioBuffer<float> dry;
    dry.makeCopyOf(buffer, true);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* out = buffer.getWritePointer(ch);
        auto* dryIn = dry.getReadPointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const auto g = smoothers.getNextValue("gain");
            const auto m = smoothers.getNextValue("mix");
            const auto wet = out[i] * g;
            out[i] = dryIn[i] + (wet - dryIn[i]) * m;
        }
    }

    analyzer.push(buffer);
}

void WaveFormProProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, destData);
}
void WaveFormProProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessorEditor* WaveFormProProcessor::createEditor() { return new WaveFormProEditor(*this); }
