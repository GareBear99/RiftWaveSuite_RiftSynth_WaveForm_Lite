#pragma once
#include <JuceHeader.h>

namespace riftwave {
class PresetBrowserComponent : public juce::Component
{
public:
    std::function<void(const juce::String&)> onPresetChosen;

    PresetBrowserComponent();
    void setItems(const juce::StringArray& items);
    juce::String getSelectedName() const;
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    void rebuildVisibleItems();
    juce::StringArray allItems, visibleItems;
    juce::Label title;
    juce::TextEditor filter;
    juce::ComboBox list;
};
}
