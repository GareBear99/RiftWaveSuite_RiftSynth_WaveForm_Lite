#pragma once
#include <JuceHeader.h>

namespace riftwave {
class SeedStripComponent : public juce::Component
{
public:
    std::function<void()> onMutate8;
    std::function<void()> onRebuild;
    std::function<void()> onSavePreset;

    SeedStripComponent();
    void setSeedCode(const juce::String& code);
    void setStatusText(const juce::String& text);
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    juce::TextEditor seedCode;
    juce::TextButton mutate8, rebuild, savePreset, copyCode;
    juce::Label status;
};
}
