#include "PluginProcessor.h"
#include "PluginEditor.h"

WaveFormRiftSynthLiteProcessor::WaveFormRiftSynthLiteProcessor()
: juce::AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
  apvts(*this, nullptr, "STATE", riftwave::SharedParameters::createLayout(riftwave::ProductTier::WaveFormRiftSynthLite))
{}

void WaveFormRiftSynthLiteProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    analyzer.prepare(sampleRate, samplesPerBlock);
    voices.prepare(sampleRate);
    scheduler.prepare(sampleRate, 16);
    oversampling.prepare(2, 2);
    rebuildPattern();
}

void WaveFormRiftSynthLiteProcessor::rebuildPattern()
{
    seed.setSeed((uint32_t) apvts.getRawParameterValue("seed")->load());
    events = seed.generatePattern(1, 16, 60, 0);
    player.setPattern(events, 16);
    scheduler.resetToBarStart();
}

juce::String WaveFormRiftSynthLiteProcessor::getSeedCode() const
{
    return seed.makeSeedCode(120, 1, 0);
}

riftwave::SessionDebugSnapshot WaveFormRiftSynthLiteProcessor::getDebugSnapshot() const
{
    riftwave::SessionDebugSnapshot s;
    const auto snap = analyzer.getSnapshot();
    s.bpm = transport.get().bpm;
    s.hostPlaying = transport.get().isPlaying;
    s.currentStep = scheduler.getSnapshot().currentStep;
    s.peak = snap.peak;
    s.rms = snap.rms;
    s.seedCode = getSeedCode();
    s.lastUiCue = uiSfx.getLastCueName();
    return s;
}

void WaveFormRiftSynthLiteProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    transport.updateFrom(getPlayHead());
    analyzer.setBeautyMode(true);

    scheduler.setTransportBpm(120.0);
    scheduler.setHostSynced(false);
    scheduler.processBlock(buffer.getNumSamples(), [this](int step, int, bool barStart)
    {
        if (barStart)
            uiSfx.trigger(riftwave::UiSfxEngine::Cue::Tick);

        player.handleStep(step, 0,
            [this](int note, float velocity, int)
            {
                voices.noteOn(note, velocity);
            },
            [this](int note, int)
            {
                voices.noteOff(note);
            });
    });

    oversampling.processBlock(buffer, [this](juce::AudioBuffer<float>& work)
    {
        for (int i = 0; i < work.getNumSamples(); ++i)
        {
            const auto s = voices.render() * 0.12f;
            for (int ch = 0; ch < work.getNumChannels(); ++ch)
                work.setSample(ch, i, s);
        }
    });

    analyzer.push(buffer);
}

void WaveFormRiftSynthLiteProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, destData);
}
void WaveFormRiftSynthLiteProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
    rebuildPattern();
}
juce::AudioProcessorEditor* WaveFormRiftSynthLiteProcessor::createEditor() { return new WaveFormRiftSynthLiteEditor(*this); }
