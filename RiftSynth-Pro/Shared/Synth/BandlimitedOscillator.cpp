#include "BandlimitedOscillator.h"
#include "SimpleVoice.h"

namespace riftwave {

void BandlimitedOscillator::prepare (double newSampleRate) noexcept
{
    sampleRate = juce::jmax (1.0, newSampleRate);
    phase = 0.0;
    integrator = 0.0f;
    setFrequency (frequencyHz);
}

void BandlimitedOscillator::setFrequency (float hz) noexcept
{
    frequencyHz = juce::jmax (1.0f, hz);
    phaseDelta = juce::jlimit (0.0, 0.49, (double) frequencyHz / sampleRate);
}

float BandlimitedOscillator::nextPhase() noexcept
{
    auto out = (float) phase;
    phase += phaseDelta;
    while (phase >= 1.0)
        phase -= 1.0;
    return out;
}

float BandlimitedOscillator::renderSine() noexcept
{
    const auto p = nextPhase();
    return std::sin (juce::MathConstants<float>::twoPi * p);
}

float BandlimitedOscillator::renderSaw() noexcept
{
    const auto p = nextPhase();
    auto value = 2.0f * p - 1.0f;
    value -= polyBlep (p, (float) phaseDelta);
    return value;
}

float BandlimitedOscillator::renderSquare() noexcept
{
    const auto p = nextPhase();
    auto value = p < pulseWidth ? 1.0f : -1.0f;
    value += polyBlep (p, (float) phaseDelta);
    auto secondEdge = std::fmod (p - pulseWidth + 1.0f, 1.0f);
    value -= polyBlep (secondEdge, (float) phaseDelta);
    return value;
}

float BandlimitedOscillator::renderTriangle() noexcept
{
    auto square = renderSquare();
    integrator += (2.0f * (float) phaseDelta) * square;
    integrator = juce::jlimit (-1.0f, 1.0f, integrator);
    return integrator;
}

float BandlimitedOscillator::renderSample() noexcept
{
    if (driftAmount > 0.0f && rng.nextFloat() < 0.0005f)
    {
        const auto cents = juce::jmap (rng.nextFloat(), -1.0f, 1.0f) * 3.0f * driftAmount;
        const auto drifted = frequencyHz * std::pow (2.0f, cents / 1200.0f);
        phaseDelta = juce::jlimit (0.0, 0.49, (double) drifted / sampleRate);
    }

    switch (shape)
    {
        case OscShape::Sine:     return renderSine();
        case OscShape::Saw:      return renderSaw();
        case OscShape::Square:   return renderSquare();
        case OscShape::Triangle: return renderTriangle();
    }

    return renderSine();
}

} // namespace riftwave
