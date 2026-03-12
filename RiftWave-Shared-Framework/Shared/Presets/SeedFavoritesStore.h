#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct FavoriteSeed
{
    uint32_t seed = 1;
    juce::String code;
    juce::String note;
};

class SeedFavoritesStore
{
public:
    explicit SeedFavoritesStore(juce::File backingFile) : file(std::move(backingFile)) {}

    juce::Array<FavoriteSeed> load() const
    {
        juce::Array<FavoriteSeed> out;
        if (!file.existsAsFile())
            return out;

        auto parsed = juce::JSON::parse(file);
        if (auto* arr = parsed.getArray())
        {
            for (const auto& item : *arr)
            {
                if (auto* obj = item.getDynamicObject())
                {
                    FavoriteSeed fav;
                    fav.seed = (uint32_t) (int) obj->getProperty("seed");
                    fav.code = obj->getProperty("code").toString();
                    fav.note = obj->getProperty("note").toString();
                    out.add(fav);
                }
            }
        }
        return out;
    }

    void save(const juce::Array<FavoriteSeed>& items) const
    {
        juce::Array<juce::var> arr;
        for (const auto& item : items)
        {
            auto* obj = new juce::DynamicObject();
            obj->setProperty("seed", (int) item.seed);
            obj->setProperty("code", item.code);
            obj->setProperty("note", item.note);
            arr.add(juce::var(obj));
        }
        file.replaceWithText(juce::JSON::toString(juce::var(arr), true));
    }

private:
    juce::File file;
};
}
