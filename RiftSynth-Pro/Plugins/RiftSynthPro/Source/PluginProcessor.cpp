#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../../Shared/Export/MidiExportStub.h"
#include "../../../Shared/Presets/UserPresetStore.h"

RiftSynthProProcessor::RiftSynthProProcessor()
: juce::AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
  apvts(*this, nullptr, "STATE", riftwave::SharedParameters::createLayout(riftwave::ProductTier::RiftSynthPro))
{}

void RiftSynthProProcessor::prepareToPlay(double sampleRate, int)
{
    sr = sampleRate;
    voices.prepare(sr);
    scheduler.prepare(sr, 16);
    oversampling.prepare(2, 2);
    smoothers.prepare(sr);
    smoothers.setCurrentAndTarget("gain", apvts.getRawParameterValue("gain")->load());
    smoothers.setCurrentAndTarget("cutoff", apvts.getRawParameterValue("cutoff")->load());
    smoothers.setCurrentAndTarget("resonance", apvts.getRawParameterValue("resonance")->load());
    rebuildPattern();
}

void RiftSynthProProcessor::rebuildPattern()
{
    seed.setSeed((uint32_t) apvts.getRawParameterValue("seed")->load());
    const auto lanes = (int) apvts.getRawParameterValue("lanes")->load();
    const auto scale = (int) apvts.getRawParameterValue("scale")->load();
    events = seed.generatePattern(lanes, 16, 48, scale);
    patternPlayer.setPattern(events, 16);
    exportedPreview = riftwave::MidiExportStub::makeSequence(events, juce::jmax(1.0, transport.get().bpm));
    currentStep = 0;
    scheduler.resetToBarStart();
    uiSfx.trigger(riftwave::UiSfxEngine::Cue::Rise);
}

juce::String RiftSynthProProcessor::getSeedCode() const
{
    return seed.makeSeedCode((int) transport.get().bpm, (int) apvts.getRawParameterValue("lanes")->load(),
                             (int) apvts.getRawParameterValue("scale")->load());
}

juce::StringArray RiftSynthProProcessor::buildVariationPreview() const
{
    auto vars = seed.generateVariations(8,
        (uint32_t) apvts.getRawParameterValue("mutateAmount")->load(),
        (int) apvts.getRawParameterValue("lanes")->load(), 16, 48,
        (int) apvts.getRawParameterValue("scale")->load(),
        apvts.getRawParameterValue("lockBass")->load() > 0.5f,
        apvts.getRawParameterValue("lockHarmony")->load() > 0.5f);

    juce::StringArray out;
    for (const auto& v : vars) out.add(v.code);
    return out;
}

juce::File RiftSynthProProcessor::exportMidiPreviewFile() const
{
    auto dir = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("RiftWaveSuite");
    dir.createDirectory();
    auto file = dir.getChildFile("RiftSynthPro_" + getSeedCode().retainCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-") + ".mid");
    riftwave::MidiExportStub::writeMidiFile(file, events, juce::jmax(1.0, transport.get().bpm));
    return file;
}

bool RiftSynthProProcessor::exportWavPreviewFile(juce::File& outFile)
{
    juce::AudioBuffer<float> preview(2, (int) (sr * 4.0));
    preview.clear();
    for (int i = 0; i < preview.getNumSamples(); ++i)
    {
        const auto synth = voices.render() * 0.18f;
        preview.setSample(0, i, synth);
        preview.setSample(1, i, synth);
    }
    return riftwave::WavPreviewExporter::writeTempPreview("RiftSynthPro", preview, sr, outFile);
}

bool RiftSynthProProcessor::saveUserPreset(const juce::String& presetName)
{
    uiSfx.trigger(riftwave::UiSfxEngine::Cue::Bloom);
    return riftwave::UserPresetStore::savePreset("RiftSynthPro", presetName, apvts);
}

bool RiftSynthProProcessor::loadUserPreset(const juce::String& presetName)
{
    const bool ok = riftwave::UserPresetStore::loadPreset("RiftSynthPro", presetName, apvts);
    if (ok)
    {
        rebuildPattern();
        uiSfx.trigger(riftwave::UiSfxEngine::Cue::Click);
    }
    return ok;
}

juce::StringArray RiftSynthProProcessor::getUserPresetNames() const
{
    return riftwave::UserPresetStore::listPresetNames("RiftSynthPro");
}

riftwave::SessionDebugSnapshot RiftSynthProProcessor::getDebugSnapshot() const
{
    riftwave::SessionDebugSnapshot s;
    s.bpm = transport.get().bpm;
    s.hostPlaying = transport.get().isPlaying;
    s.currentStep = scheduler.getSnapshot().currentStep;
    s.seedCode = getSeedCode();
    s.lastUiCue = uiSfx.getLastCueName();
    return s;
}

void RiftSynthProProcessor::updateVoiceTargets()
{
    smoothers.setTarget("cutoff", apvts.getRawParameterValue("cutoff")->load());
    smoothers.setTarget("resonance", apvts.getRawParameterValue("resonance")->load());
    smoothers.setTarget("gain", apvts.getRawParameterValue("gain")->load());
    voices.setEnvelope(apvts.getRawParameterValue("attack")->load(), apvts.getRawParameterValue("decay")->load(),
                       apvts.getRawParameterValue("sustain")->load(), apvts.getRawParameterValue("release")->load());
    voices.setShape((riftwave::OscShape) (int) apvts.getRawParameterValue("wave")->load());
    voices.setDriftAmount(0.08f);
    voices.setSubMix(0.18f);
    voices.setCutoff(smoothers.getNextValue("cutoff"));
    voices.setResonance(smoothers.getNextValue("resonance"));
}

void RiftSynthProProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    transport.updateFrom(getPlayHead());
    updateVoiceTargets();

    const auto hostSync = apvts.getRawParameterValue("hostSync")->load() > 0.5f;
    const auto bpm = (hostSync && transport.get().bpm > 1.0) ? transport.get().bpm : 120.0;
    scheduler.setTransportBpm(bpm);
    scheduler.setHostSynced(hostSync);
    scheduler.syncToTransport(transport.get());

    if (transport.get().isPlaying && ! wasPlaying)
    {
        scheduler.resetToBarStart();
        patternPlayer.reset();
        uiSfx.trigger(riftwave::UiSfxEngine::Cue::Spark);
    }
    else if (! transport.get().isPlaying && wasPlaying)
    {
        patternPlayer.allNotesOff([this](int midiNote, int)
        {
            voices.noteOff(midiNote);
        });
        voices.allNotesOff();
    }
    wasPlaying = transport.get().isPlaying;

    if (transport.get().isPlaying || ! hostSync)
    {
        scheduler.processBlock(buffer.getNumSamples(), [this, &midi](int step, int offset, bool isBarStart)
        {
            currentStep = step;
            if (isBarStart)
                uiSfx.trigger(riftwave::UiSfxEngine::Cue::Tick);

            patternPlayer.handleStep(step, offset,
                [this, &midi](int note, float velocity, int sampleOffset)
                {
                    voices.noteOn(note, velocity);
                    midi.addEvent(juce::MidiMessage::noteOn(1, note, velocity), sampleOffset);
                },
                [this, &midi](int note, int sampleOffset)
                {
                    voices.noteOff(note);
                    midi.addEvent(juce::MidiMessage::noteOff(1, note), sampleOffset);
                });
        });
    }

    oversampling.processBlock(buffer, [this](juce::AudioBuffer<float>& work)
    {
        for (int i = 0; i < work.getNumSamples(); ++i)
        {
            const auto synth = voices.render() * 0.22f * smoothers.getNextValue("gain");
            for (int ch = 0; ch < work.getNumChannels(); ++ch)
                work.setSample(ch, i, synth);
        }
    });
}

void RiftSynthProProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml()) copyXmlToBinary(*xml, destData);
}
void RiftSynthProProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
    rebuildPattern();
}
juce::AudioProcessorEditor* RiftSynthProProcessor::createEditor() { return new RiftSynthProEditor(*this); }
