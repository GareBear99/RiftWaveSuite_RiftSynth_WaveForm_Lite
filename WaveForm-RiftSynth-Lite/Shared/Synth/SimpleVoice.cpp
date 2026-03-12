#include "SimpleVoice.h"

namespace riftwave {
void SimpleVoice::prepare(double sampleRate)
{
    sr = sampleRate;
    adsr.setSampleRate(sr);
    cutoff.reset(sr, 0.03);
    gainTrim.reset(sr, 0.01);
    filter.reset();
    filter.prepare({ sampleRate, 64, 1 });
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    osc.prepare(sr);
    subOsc.prepare(sr);
    subOsc.setShape(OscShape::Sine);
    osc.setDriftAmount(driftAmount);
}

void SimpleVoice::setEnvelope(float a, float d, float s, float r)
{
    adsrParams.attack = a;
    adsrParams.decay = d;
    adsrParams.sustain = s;
    adsrParams.release = r;
    adsr.setParameters(adsrParams);
}

void SimpleVoice::setCutoff(float cutoffHz) { cutoff.setTargetValue(cutoffHz); }
void SimpleVoice::setResonance(float value) { filter.setResonance(juce::jlimit(0.1f, 1.2f, value)); }

void SimpleVoice::noteOn(int midiNote, float velocity)
{
    baseFrequencyHz = (float) juce::MidiMessage::getMidiNoteInHertz(midiNote);
    osc.setFrequency(baseFrequencyHz);
    subOsc.setFrequency(baseFrequencyHz * 0.5f);
    level = juce::jlimit(0.0f, 1.0f, velocity);
    gainTrim.setCurrentAndTargetValue(juce::jmap(level, 0.0f, 1.0f, 0.75f, 1.0f));
    adsr.noteOn();
}

void SimpleVoice::noteOff() { adsr.noteOff(); }

float SimpleVoice::render()
{
    filter.setCutoffFrequency(cutoff.getNextValue());
    const auto env = adsr.getNextSample();
    const auto main = osc.renderSample();
    const auto sub = subOsc.renderSample() * subMix;
    auto v = (main + sub) * level * env * gainTrim.getNextValue();
    return filter.processSample(v);
}

void VoiceManager::prepare(double sampleRate)
{
    for (auto& v : voices) v.voice.prepare(sampleRate);
}
void VoiceManager::setEnvelope(float a, float d, float s, float r) { for (auto& v : voices) v.voice.setEnvelope(a,d,s,r); }
void VoiceManager::setCutoff(float cutoffHz) { for (auto& v : voices) v.voice.setCutoff(cutoffHz); }
void VoiceManager::setResonance(float resonance) { for (auto& v : voices) v.voice.setResonance(resonance); }
void VoiceManager::setShape(OscShape shape) { for (auto& v : voices) v.voice.setShape(shape); }
void VoiceManager::setDriftAmount(float amount) { for (auto& v : voices) v.voice.setDriftAmount(amount); }
void VoiceManager::setSubMix(float amount) { for (auto& v : voices) v.voice.setSubMix(amount); }

void VoiceManager::noteOn(int midiNote, float velocity)
{
    auto it = std::min_element(voices.begin(), voices.end(), [](const Slot& a, const Slot& b){ return (a.active ? a.age : 0) < (b.active ? b.age : 0); });
    if (auto free = std::find_if(voices.begin(), voices.end(), [](const Slot& s){ return !s.active || !s.voice.isActive(); }); free != voices.end()) it = free;
    it->midiNote = midiNote;
    it->active = true;
    it->age = ++counter;
    it->voice.noteOn(midiNote, velocity);
}

void VoiceManager::noteOff(int midiNote)
{
    for (auto& slot : voices)
        if (slot.active && slot.midiNote == midiNote)
            slot.voice.noteOff();
}

void VoiceManager::allNotesOff()
{
    for (auto& slot : voices)
    {
        slot.voice.noteOff();
        slot.active = false;
    }
}

float VoiceManager::render()
{
    float sum = 0.0f;
    for (auto& slot : voices)
    {
        if (slot.active)
        {
            sum += slot.voice.render();
            if (! slot.voice.isActive())
                slot.active = false;
        }
    }
    return sum * 0.15f;
}
}
