#pragma once
#include <JuceHeader.h>

namespace riftwave {
class NeonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    NeonLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff3be0ff));
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff00ffb7));
        setColour(juce::Label::textColourId, juce::Colour(0xffd7f8ff));
    }
};
}
