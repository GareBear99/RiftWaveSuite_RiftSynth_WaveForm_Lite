#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct ThemePreset
{
    juce::String name;
    juce::Colour background;
    juce::Colour primary;
    juce::Colour accent;
};

struct ThemeBank
{
    static juce::Array<ThemePreset> makeDefaultThemes()
    {
        return {
            {"Neon Rift", juce::Colour(0xff050912), juce::Colour(0xff00ffb7), juce::Colour(0xff6d5cff)},
            {"Arc Blue", juce::Colour(0xff040915), juce::Colour(0xff41e0ff), juce::Colour(0xff99f2ff)},
            {"Void Amber", juce::Colour(0xff0b0910), juce::Colour(0xffffb347), juce::Colour(0xffffe0a1)},
            {"Blood Pulse", juce::Colour(0xff12070a), juce::Colour(0xffff4f6f), juce::Colour(0xffffb0ba)}
        };
    }
};
}
