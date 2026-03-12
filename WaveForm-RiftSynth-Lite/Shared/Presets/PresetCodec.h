#pragma once
#include <JuceHeader.h>

namespace riftwave {
class PresetCodec
{
public:
    static juce::ValueTree stateToPreset(const juce::AudioProcessorValueTreeState& apvts, const juce::String& presetName)
    {
        auto tree = apvts.copyState();
        tree.setProperty("presetName", presetName, nullptr);
        tree.setProperty("presetVersion", 2, nullptr);
        tree.setProperty("format", "RiftWavePreset", nullptr);
        return tree;
    }

    static bool applyPreset(juce::AudioProcessorValueTreeState& apvts, const juce::ValueTree& tree)
    {
        if (! tree.isValid())
            return false;
        if (tree.hasProperty("format") && tree["format"].toString() != "RiftWavePreset")
            return false;
        apvts.replaceState(tree.createCopy());
        return true;
    }

    static juce::String toJsonString(const juce::ValueTree& tree)
    {
        return juce::JSON::toString(juce::var(tree), true);
    }

    static juce::ValueTree fromJsonString(const juce::String& json)
    {
        const auto parsed = juce::JSON::parse(json);
        if (auto* obj = parsed.getDynamicObject())
            return juce::ValueTree::fromVar(parsed);
        return {};
    }

    static bool writePresetFile(const juce::File& file, const juce::AudioProcessorValueTreeState& apvts, const juce::String& presetName)
    {
        if (! file.getParentDirectory().createDirectory())
            return false;
        return file.replaceWithText(toJsonString(stateToPreset(apvts, presetName)));
    }

    static bool readPresetFile(const juce::File& file, juce::AudioProcessorValueTreeState& apvts)
    {
        if (! file.existsAsFile())
            return false;
        return applyPreset(apvts, fromJsonString(file.loadFileAsString()));
    }
};
}
