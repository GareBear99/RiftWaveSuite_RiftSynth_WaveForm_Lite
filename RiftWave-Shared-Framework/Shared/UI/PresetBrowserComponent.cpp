#include "PresetBrowserComponent.h"

namespace riftwave {
PresetBrowserComponent::PresetBrowserComponent()
{
    addAndMakeVisible(title);
    addAndMakeVisible(filter);
    addAndMakeVisible(list);

    title.setText("Presets", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centredLeft);
    title.setColour(juce::Label::textColourId, juce::Colour(0xffd7f8ff));

    filter.setTextToShowWhenEmpty("Filter presets", juce::Colours::grey);
    filter.onTextChange = [this] { rebuildVisibleItems(); };
    list.onChange = [this]
    {
        if (onPresetChosen)
            onPresetChosen(list.getText());
    };
}

void PresetBrowserComponent::setItems(const juce::StringArray& items)
{
    allItems = items;
    rebuildVisibleItems();
}

juce::String PresetBrowserComponent::getSelectedName() const
{
    return list.getText();
}

void PresetBrowserComponent::rebuildVisibleItems()
{
    visibleItems.clear();
    const auto needle = filter.getText().trim().toLowerCase();
    for (auto& item : allItems)
        if (needle.isEmpty() || item.toLowerCase().contains(needle))
            visibleItems.add(item);

    list.clear(juce::dontSendNotification);
    for (int i = 0; i < visibleItems.size(); ++i)
        list.addItem(visibleItems[i], i + 1);
    if (visibleItems.size() > 0)
        list.setSelectedItemIndex(0, juce::dontSendNotification);
}

void PresetBrowserComponent::resized()
{
    auto area = getLocalBounds().reduced(6);
    title.setBounds(area.removeFromTop(20));
    filter.setBounds(area.removeFromTop(24));
    area.removeFromTop(4);
    list.setBounds(area);
}

void PresetBrowserComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff0e1523));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10.0f);
    g.setColour(juce::Colour(0xff35507e));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 10.0f, 1.0f);
}
}
