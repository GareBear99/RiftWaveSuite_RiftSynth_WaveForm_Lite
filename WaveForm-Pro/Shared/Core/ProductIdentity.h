#pragma once
#include <JuceHeader.h>
#include "ProductTypes.h"

namespace riftwave {
struct ProductIdentity
{
    juce::String companyName;
    juce::String companyCode;
    juce::String pluginName;
    juce::String pluginCode;
    juce::String bundleId;
    juce::String auMainType;
    juce::String auSubType;
    juce::String vst3Category;
    juce::String installStem;
};

inline ProductIdentity makeIdentity(ProductTier tier)
{
    switch (tier)
    {
        case ProductTier::Lite:
            return { "GareBear Audio", "GBRA", "WaveForm / RiftSynth Lite", "RWLT", "com.garebearaudio.riftwave.lite", "aumu", "RwLt", "Fx|Instrument", "WaveFormRiftSynthLite" };
        case ProductTier::WaveFormPro:
            return { "GareBear Audio", "GBRA", "WaveForm Pro", "RWFP", "com.garebearaudio.riftwave.waveformpro", "aufx", "RwFp", "Fx|Analyzer", "WaveFormPro" };
        case ProductTier::RiftSynthPro:
        default:
            return { "GareBear Audio", "GBRA", "RiftSynth Pro", "RWRS", "com.garebearaudio.riftwave.riftsynthpro", "aumu", "RwRs", "Instrument|Synth", "RiftSynthPro" };
    }
}
}
