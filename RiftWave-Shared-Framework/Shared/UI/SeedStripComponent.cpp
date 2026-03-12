#include "SeedStripComponent.h"

namespace riftwave {
SeedStripComponent::SeedStripComponent()
{
    addAndMakeVisible(seedCode);
    addAndMakeVisible(mutate8);
    addAndMakeVisible(rebuild);
    addAndMakeVisible(savePreset);
    addAndMakeVisible(copyCode);
    addAndMakeVisible(status);

    seedCode.setReadOnly(true);
    seedCode.setTextToShowWhenEmpty("SEED-CODE", juce::Colours::grey);
    status.setText("Ready", juce::dontSendNotification);
    status.setColour(juce::Label::textColourId, juce::Colour(0xffd7f8ff));

    mutate8.setButtonText("Mutate x8");
    rebuild.setButtonText("Rebuild");
    savePreset.setButtonText("Save");
    copyCode.setButtonText("Copy");

    mutate8.onClick = [this] { setStatusText("Generated 8 variations"); if (onMutate8) onMutate8(); };
    rebuild.onClick = [this] { setStatusText("Pattern rebuilt"); if (onRebuild) onRebuild(); };
    savePreset.onClick = [this] { setStatusText("Preset saved"); if (onSavePreset) onSavePreset(); };
    copyCode.onClick = [this]
    {
        juce::SystemClipboard::copyTextToClipboard(seedCode.getText());
        setStatusText("Seed copied");
    };
}

void SeedStripComponent::setSeedCode(const juce::String& code)
{
    seedCode.setText(code, juce::dontSendNotification);
}

void SeedStripComponent::setStatusText(const juce::String& text)
{
    status.setText(text, juce::dontSendNotification);
}

void SeedStripComponent::resized()
{
    auto area = getLocalBounds().reduced(4);
    seedCode.setBounds(area.removeFromLeft(320));
    copyCode.setBounds(area.removeFromLeft(70).reduced(2));
    mutate8.setBounds(area.removeFromLeft(100).reduced(2));
    rebuild.setBounds(area.removeFromLeft(90).reduced(2));
    savePreset.setBounds(area.removeFromLeft(90).reduced(2));
    status.setBounds(area.reduced(6, 2));
}

void SeedStripComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0x1800ffb7));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
    g.setColour(juce::Colour(0x5000ffb7));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 10.0f, 1.0f);
}
}
