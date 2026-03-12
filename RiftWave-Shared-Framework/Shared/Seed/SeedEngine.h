#pragma once
#include <JuceHeader.h>
#include "../Core/ProductTypes.h"
#include <vector>

namespace riftwave {
struct LaneEvent {
    LaneRole role = LaneRole::Bass;
    int laneIndex = 0;
    int startStep = 0;
    int midiNote = 60;
    float velocity = 0.8f;
    int lengthSteps = 1;
};

struct PatternVariant
{
    juce::String code;
    uint32_t seed = 1;
    std::vector<LaneEvent> events;
};

class SeedEngine {
public:
    void setSeed(uint32_t newSeed) noexcept { seed = newSeed == 0 ? 1u : newSeed; }
    uint32_t getSeed() const noexcept { return seed; }
    std::vector<LaneEvent> generateLane(int laneIndex, int steps, int rootMidi, int scaleIndex) const;
    std::vector<LaneEvent> generatePattern(int laneCount, int steps, int rootMidi, int scaleIndex) const;
    std::vector<PatternVariant> generateVariations(int count, uint32_t amount, int laneCount, int steps, int rootMidi, int scaleIndex, bool lockBass, bool lockHarmony) const;
    juce::String makeSeedCode(int tempo, int lanes, int scaleIndex) const;
    uint32_t mutate(uint32_t amount) const noexcept;

private:
    uint32_t xorshift(uint32_t& state) const noexcept;
    int quantizeToScale(int degree, int scaleIndex) const noexcept;
    LaneRole roleForLane(int laneIndex) const noexcept;
    uint32_t seed = 424242u;
};
}
