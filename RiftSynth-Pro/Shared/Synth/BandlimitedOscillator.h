#pragma once
#include <JuceHeader.h>

namespace riftwave {

enum class OscShape;

class BandlimitedOscillator
{
public:
    void prepare (double sampleRate) noexcept;
    void setFrequency (float hz) noexcept;
    void setShape (OscShape newShape) noexcept { shape = newShape; }
    void setPulseWidth (float newPw) noexcept { pulseWidth = juce::jlimit (0.05f, 0.95f, newPw); }
    void setDriftAmount (float amount) noexcept { driftAmount = juce::jlimit (0.0f, 1.0f, amount); }
    float renderSample() noexcept;

private:
    static inline float polyBlep (float t, float dt) noexcept
    {
        if (t < dt)
        {
            t /= dt;
            return t + t - t * t - 1.0f;
        }

        if (t > 1.0f - dt)
        {
            t = (t - 1.0f) / dt;
            return t * t + t + t + 1.0f;
        }

        return 0.0f;
    }

    float nextPhase() noexcept;
    float renderSaw() noexcept;
    float renderSquare() noexcept;
    float renderTriangle() noexcept;
    float renderSine() noexcept;

    double sampleRate = 44100.0;
    double phase = 0.0;
    double phaseDelta = 0.0;
    float frequencyHz = 110.0f;
    float pulseWidth = 0.5f;
    float driftAmount = 0.0f;
    float integrator = 0.0f;
    juce::Random rng;
    OscShape shape;
};

} // namespace riftwave
