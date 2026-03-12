#pragma once
#include <JuceHeader.h>
#include "ParameterBinding.h"

namespace riftwave {
struct AttachmentSet
{
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachments;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;

    void attach(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::Slider& slider)
    {
        if (auto* p = apvts.getParameter(id))
            slider.getProperties().set("paramPtr", dynamic_cast<juce::RangedAudioParameter*>(p));
        sliderAttachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(apvts, id, slider));
    }

    void attach(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::ComboBox& box)
    {
        comboAttachments.add(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, id, box));
    }

    void attach(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::Button& button)
    {
        buttonAttachments.add(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, id, button));
    }
};
}
