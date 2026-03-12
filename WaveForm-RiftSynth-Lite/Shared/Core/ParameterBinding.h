#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct SliderBinding { std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment; };
struct ComboBinding  { std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment; };
struct ButtonBinding { std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment; };

inline void styleRotary(juce::Slider& slider, const juce::String& suffix = {})
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);
    slider.setPopupDisplayEnabled(true, false, nullptr);
    slider.setNumDecimalPlacesToDisplay(2);
    if (suffix.isNotEmpty())
        slider.setTextValueSuffix(" " + suffix);
    slider.onDoubleClick = [&slider]
    {
        if (auto* rp = dynamic_cast<juce::RangedAudioParameter*>(slider.getProperties()["paramPtr"].getObject()))
            slider.setValue(rp->convertFrom0to1(rp->getDefaultValue()), juce::sendNotificationSync);
    };
}

inline void styleLinear(juce::Slider& slider, const juce::String& suffix = {})
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 20);
    slider.setPopupDisplayEnabled(true, false, nullptr);
    if (suffix.isNotEmpty())
        slider.setTextValueSuffix(" " + suffix);
}

inline SliderBinding bind(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::Slider& slider)
{
    if (auto* p = apvts.getParameter(id))
        slider.getProperties().set("paramPtr", dynamic_cast<juce::RangedAudioParameter*>(p));
    return { std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, id, slider) };
}

inline ComboBinding bind(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::ComboBox& box)
{
    return { std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, id, box) };
}

inline ButtonBinding bind(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::Button& button)
{
    return { std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, id, button) };
}
}
