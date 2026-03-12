#pragma once
#include <JuceHeader.h>
#include "PresetCodec.h"

namespace riftwave {
class UserPresetStore
{
public:
    static juce::File getRootDirectory()
    {
        auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                     .getChildFile("GaryDoman")
                     .getChildFile("RiftWaveSuite")
                     .getChildFile("Presets");
        dir.createDirectory();
        return dir;
    }

    static juce::File getProductDirectory(const juce::String& productName)
    {
        auto dir = getRootDirectory().getChildFile(productName);
        dir.createDirectory();
        return dir;
    }

    static juce::StringArray listPresetNames(const juce::String& productName)
    {
        juce::StringArray out;
        auto dir = getProductDirectory(productName);
        for (const auto& file : dir.findChildFiles(juce::File::findFiles, false, "*.rwspreset"))
            out.add(file.getFileNameWithoutExtension());
        out.sort(true);
        return out;
    }

    static bool savePreset(const juce::String& productName,
                           const juce::String& presetName,
                           const juce::AudioProcessorValueTreeState& apvts)
    {
        if (presetName.trim().isEmpty())
            return false;
        auto file = getProductDirectory(productName).getChildFile(presetName + ".rwspreset");
        return PresetCodec::writePresetFile(file, apvts, presetName);
    }

    static bool loadPreset(const juce::String& productName,
                           const juce::String& presetName,
                           juce::AudioProcessorValueTreeState& apvts)
    {
        auto file = getProductDirectory(productName).getChildFile(presetName + ".rwspreset");
        return PresetCodec::readPresetFile(file, apvts);
    }
};
}
