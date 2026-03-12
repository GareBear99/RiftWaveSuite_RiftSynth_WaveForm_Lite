#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct PresetMeta
{
    juce::String name;
    juce::String category;
    juce::String description;
    juce::String relativePath;
};

class PresetBank
{
public:
    static juce::Array<PresetMeta> makeWaveFormBank()
    {
        return {
            { "Master Bus Clean", "Mix", "Balanced analyzer with low visual clutter", "Plugins/WaveFormPro/Presets/MasterBusClean.rwspreset" },
            { "Orbit Heat", "Visual", "Cinematic orbit scene with heat-style movement", "Plugins/WaveFormPro/Presets/OrbitHeat.rwspreset" },
            { "Stereo Tunnel", "Visual", "Wide stereo scene for stream/demo moments", "Plugins/WaveFormPro/Presets/StereoTunnel.rwspreset" }
        };
    }

    static juce::Array<PresetMeta> makeRiftSynthBank()
    {
        return {
            { "Dark Arp", "Seed", "Fast minor arpeggio seed", "Plugins/RiftSynthPro/Presets/DarkArp.rwspreset" },
            { "Neon Minor", "Seed", "Focused neon synth phrase", "Plugins/RiftSynthPro/Presets/NeonMinor.rwspreset" },
            { "Seed Choir", "Pad", "Slower atmospheric seed texture", "Plugins/RiftSynthPro/Presets/SeedChoir.rwspreset" }
        };
    }

    static juce::Array<PresetMeta> makeLiteBank()
    {
        return {
            { "INIT", "Starter", "Neutral free-plugin init", "Plugins/WaveFormRiftSynthLite/Presets/INIT.rwspreset" },
            { "Glow Seed", "Starter", "Small wow-factor preview patch", "Plugins/WaveFormRiftSynthLite/Presets/GlowSeed.rwspreset" }
        };
    }
};
}
