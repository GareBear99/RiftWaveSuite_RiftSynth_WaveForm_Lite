#pragma once
#include <JuceHeader.h>
#include "ProductTypes.h"

namespace riftwave {
struct SharedParameters
{
    static juce::AudioProcessorValueTreeState::ParameterLayout createLayout(ProductTier tier)
    {
        using APF = juce::AudioParameterFloat;
        using APC = juce::AudioParameterChoice;
        using API = juce::AudioParameterInt;
        using APB = juce::AudioParameterBool;

        auto hzRange = juce::NormalisableRange<float>(50.0f, 18000.0f, 0.01f, 0.28f);
        auto percentRange = juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f);

        std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
        p.push_back(std::make_unique<APF>("gain", "Gain", juce::NormalisableRange<float>(0.0f, 2.0f, 0.001f), 1.0f));
        p.push_back(std::make_unique<APF>("mix", "Mix", percentRange, 1.0f));
        p.push_back(std::make_unique<APF>("sensitivity", "Sensitivity", juce::NormalisableRange<float>(0.10f, 4.0f, 0.001f, 0.4f), 1.0f));
        p.push_back(std::make_unique<APF>("smooth", "Smooth", juce::NormalisableRange<float>(0.0f, 0.995f, 0.001f), 0.82f));
        p.push_back(std::make_unique<APB>("beautyMode", "Beauty Mode", true));
        p.push_back(std::make_unique<APB>("uiSfx", "UI SFX", true));

        p.push_back(std::make_unique<API>("seed", "Seed", 1, 999999, 424242));
        p.push_back(std::make_unique<API>("mutateAmount", "Mutate Amount", 1, 64, 8));
        p.push_back(std::make_unique<APC>("scale", "Scale", juce::StringArray{"Minor","Major","Dorian","Phrygian"}, 0));
        p.push_back(std::make_unique<API>("lanes", "Lanes", 1, tier == ProductTier::RiftSynthPro ? 4 : 1, tier == ProductTier::RiftSynthPro ? 4 : 1));
        p.push_back(std::make_unique<APB>("lockBass", "Lock Bass", false));
        p.push_back(std::make_unique<APB>("lockHarmony", "Lock Harmony", false));
        p.push_back(std::make_unique<APB>("hostSync", "Host Sync", true));

        p.push_back(std::make_unique<APF>("cutoff", "Cutoff", hzRange, 2500.0f));
        p.push_back(std::make_unique<APF>("resonance", "Resonance", juce::NormalisableRange<float>(0.10f, 1.20f, 0.001f), 0.30f));
        p.push_back(std::make_unique<APF>("attack", "Attack", juce::NormalisableRange<float>(0.001f, 2.0f, 0.0001f, 0.35f), 0.01f));
        p.push_back(std::make_unique<APF>("decay", "Decay", juce::NormalisableRange<float>(0.005f, 2.0f, 0.0001f, 0.35f), 0.14f));
        p.push_back(std::make_unique<APF>("sustain", "Sustain", percentRange, 0.65f));
        p.push_back(std::make_unique<APF>("release", "Release", juce::NormalisableRange<float>(0.005f, 4.0f, 0.0001f, 0.35f), 0.30f));

        if (tier != ProductTier::RiftSynthPro)
        {
            p.push_back(std::make_unique<APC>("scene", "Scene", juce::StringArray{"Ribbon","Orbit","Pulse","Tunnel","Ladder"}, 0));
            p.push_back(std::make_unique<APF>("density", "Density", percentRange, 0.55f));
            p.push_back(std::make_unique<APF>("width", "Width", percentRange, 0.65f));
            p.push_back(std::make_unique<APF>("peakHold", "Peak Hold", juce::NormalisableRange<float>(0.05f, 2.50f, 0.001f, 0.4f), 0.35f));
        }

        if (tier == ProductTier::RiftSynthPro || tier == ProductTier::Lite)
        {
            p.push_back(std::make_unique<APC>("wave", "Wave", juce::StringArray{"Sine","Saw","Square","Triangle"}, 1));
            p.push_back(std::make_unique<APC>("oversampling", "Oversampling", juce::StringArray{"1x","2x","4x"}, tier == ProductTier::RiftSynthPro ? 1 : 0));
        }

        return { p.begin(), p.end() };
    }
};
}
