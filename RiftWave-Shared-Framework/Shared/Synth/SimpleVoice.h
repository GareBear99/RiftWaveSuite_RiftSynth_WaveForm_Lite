#pragma once
#include <JuceHeader.h>
#include "BandlimitedOscillator.h"

namespace riftwave {
enum class OscShape { Sine = 0, Saw, Square, Triangle };

class SimpleVoice {
public:
    void prepare(double sampleRate);
    void setEnvelope(float attackSeconds, float decaySeconds, float sustainLevel, float releaseSeconds);
    void setCutoff(float cutoffHz);
    void setResonance(float value);
    void setShape(OscShape newShape) noexcept { shape = newShape; osc.setShape(newShape); subOsc.setShape(OscShape::Sine); }
    void setDriftAmount(float amount) noexcept { driftAmount = juce::jlimit(0.0f, 1.0f, amount); osc.setDriftAmount(driftAmount); }
    void setSubMix(float amount) noexcept { subMix = juce::jlimit(0.0f, 1.0f, amount); }
    void noteOn(int midiNote, float velocity);
    void noteOff();
    bool isActive() const noexcept { return adsr.isActive(); }
    float render();

private:
    double sr = 44100.0;
    float level = 0.0f;
    float baseFrequencyHz = 110.0f;
    float driftAmount = 0.02f;
    float subMix = 0.18f;
    OscShape shape = OscShape::Saw;
    BandlimitedOscillator osc;
    BandlimitedOscillator subOsc;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::SmoothedValue<float> cutoff { 2000.0f };
    juce::SmoothedValue<float> gainTrim { 1.0f };
    juce::dsp::StateVariableTPTFilter<float> filter;
};

class VoiceManager {
public:
    static constexpr int kMaxVoices = 8;
    void prepare(double sampleRate);
    void setEnvelope(float a, float d, float s, float r);
    void setCutoff(float cutoffHz);
    void setResonance(float resonance);
    void setShape(OscShape shape);
    void setDriftAmount(float amount);
    void setSubMix(float amount);
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);
    void allNotesOff();
    float render();

private:
    struct Slot { SimpleVoice voice; int midiNote = -1; bool active = false; uint64_t age = 0; };
    std::array<Slot, kMaxVoices> voices {};
    uint64_t counter = 0;
};
}
