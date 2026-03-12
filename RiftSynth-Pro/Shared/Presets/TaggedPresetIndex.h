#pragma once
#include <JuceHeader.h>

namespace riftwave {

struct TaggedPresetEntry
{
    juce::String name;
    juce::String category;
    juce::StringArray tags;
    juce::File file;
};

class TaggedPresetIndex
{
public:
    void scanPresetFolder (const juce::File& folder, const juce::String& defaultCategory)
    {
        entries.clear();
        if (! folder.isDirectory())
            return;

        for (const auto& child : folder.findChildFiles (juce::File::findFiles, false, "*.rwspreset"))
        {
            TaggedPresetEntry entry;
            entry.file = child;
            entry.name = child.getFileNameWithoutExtension();
            entry.category = defaultCategory;
            entry.tags.addTokens (entry.name.fromFirstOccurrenceOf ("_", false, false), "-", "");
            entries.push_back (entry);
        }
    }

    juce::StringArray listCategories() const
    {
        juce::StringArray out;
        for (const auto& entry : entries)
            out.addIfNotAlreadyThere (entry.category);
        return out;
    }

    const std::vector<TaggedPresetEntry>& getEntries() const noexcept { return entries; }

private:
    std::vector<TaggedPresetEntry> entries;
};

} // namespace riftwave
