#pragma once
#include <JuceHeader.h>

namespace riftwave {

class OversamplingEngine
{
public:
    void prepare (size_t channels, int factor)
    {
        const auto order = factor <= 2 ? 1 : 2;
        oversampler = std::make_unique<juce::dsp::Oversampling<float>> ((size_t) channels, (size_t) order,
                                                                        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR);
        oversampler->reset();
        oversamplingFactor = factor;
    }

    int getFactor() const noexcept { return oversamplingFactor; }

    template <typename RenderFn>
    void processBlock (juce::AudioBuffer<float>& buffer, RenderFn&& fn)
    {
        if (! oversampler || oversamplingFactor <= 1)
        {
            fn (buffer);
            return;
        }

        juce::dsp::AudioBlock<float> base (buffer);
        auto up = oversampler->processSamplesUp (base);
        juce::AudioBuffer<float> temp ((int) up.getNumChannels(), (int) up.getNumSamples());

        for (size_t ch = 0; ch < up.getNumChannels(); ++ch)
            for (size_t i = 0; i < up.getNumSamples(); ++i)
                temp.setSample ((int) ch, (int) i, up.getSample (ch, i));

        fn (temp);

        juce::dsp::AudioBlock<float> tempBlock (temp);
        for (size_t ch = 0; ch < up.getNumChannels(); ++ch)
            for (size_t i = 0; i < up.getNumSamples(); ++i)
                up.setSample (ch, i, tempBlock.getSample (ch, i));

        oversampler->processSamplesDown (base);
    }

private:
    int oversamplingFactor = 1;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
};

} // namespace riftwave
