#include "SeedEngine.h"

namespace riftwave {
uint32_t SeedEngine::xorshift(uint32_t& s) const noexcept
{
    s ^= s << 13; s ^= s >> 17; s ^= s << 5;
    return s;
}

LaneRole SeedEngine::roleForLane(int laneIndex) const noexcept
{
    switch (laneIndex % 4)
    {
        case 0: return LaneRole::Bass;
        case 1: return LaneRole::Chord;
        case 2: return LaneRole::Melody;
        default: return LaneRole::Percussion;
    }
}

int SeedEngine::quantizeToScale(int degree, int scaleIndex) const noexcept
{
    static const int scales[4][7] = {
        {0, 2, 3, 5, 7, 8, 10},
        {0, 2, 4, 5, 7, 9, 11},
        {0, 2, 3, 5, 7, 9, 10},
        {0, 1, 3, 5, 7, 8, 10}
    };
    const int* scale = scales[juce::jlimit(0, 3, scaleIndex)];
    const int octave = degree / 7;
    const int note = scale[((degree % 7) + 7) % 7];
    return note + octave * 12;
}

std::vector<LaneEvent> SeedEngine::generateLane(int laneIndex, int steps, int rootMidi, int scaleIndex) const
{
    std::vector<LaneEvent> events;
    auto state = seed + 0x9e3779b9u * (uint32_t) (laneIndex + 1);
    const auto role = roleForLane(laneIndex);

    for (int step = 0; step < steps; ++step)
    {
        const auto r = xorshift(state);
        const int gate = (int) (r % 100);
        const int threshold = role == LaneRole::Bass ? 38 : role == LaneRole::Chord ? 30 : role == LaneRole::Melody ? 48 : 20;
        if (gate > threshold) continue;

        LaneEvent e;
        e.role = role;
        e.laneIndex = laneIndex;
        e.startStep = step;
        e.lengthSteps = role == LaneRole::Chord ? 2 : ((r >> 5) & 1u ? 1 : 2);
        e.velocity = juce::jmap((float) (xorshift(state) & 255u), 0.0f, 255.0f, 0.45f, 1.00f);

        if (role == LaneRole::Percussion)
            e.midiNote = ((r >> 8) & 1u) ? 36 : ((r >> 9) & 1u ? 42 : 38);
        else
            e.midiNote = rootMidi + quantizeToScale((int) (xorshift(state) % 14u), scaleIndex)
                + (role == LaneRole::Bass ? -12 : role == LaneRole::Melody ? 12 : 0);

        events.push_back(e);
    }

    return events;
}

std::vector<LaneEvent> SeedEngine::generatePattern(int laneCount, int steps, int rootMidi, int scaleIndex) const
{
    std::vector<LaneEvent> all;
    for (int lane = 0; lane < laneCount; ++lane)
    {
        auto laneEvents = generateLane(lane, steps, rootMidi, scaleIndex);
        all.insert(all.end(), laneEvents.begin(), laneEvents.end());
    }
    std::sort(all.begin(), all.end(), [](const LaneEvent& a, const LaneEvent& b)
    {
        return std::tie(a.startStep, a.laneIndex, a.midiNote) < std::tie(b.startStep, b.laneIndex, b.midiNote);
    });
    return all;
}

std::vector<PatternVariant> SeedEngine::generateVariations(int count, uint32_t amount, int laneCount, int steps, int rootMidi, int scaleIndex, bool lockBass, bool lockHarmony) const
{
    std::vector<PatternVariant> out;
    out.reserve((size_t) juce::jmax(1, count));

    auto basePattern = generatePattern(laneCount, steps, rootMidi, scaleIndex);
    for (int i = 0; i < count; ++i)
    {
        SeedEngine variantEngine = *this;
        variantEngine.setSeed(mutate(amount + (uint32_t) i * 11u));
        auto next = variantEngine.generatePattern(laneCount, steps, rootMidi, scaleIndex);

        if (lockBass || lockHarmony)
        {
            for (auto& e : next)
            {
                for (const auto& base : basePattern)
                {
                    const bool bassLocked = lockBass && e.role == LaneRole::Bass && base.role == LaneRole::Bass && e.startStep == base.startStep;
                    const bool harmonyLocked = lockHarmony && e.role == LaneRole::Chord && base.role == LaneRole::Chord && e.startStep == base.startStep;
                    if (bassLocked || harmonyLocked)
                    {
                        e.midiNote = base.midiNote;
                        e.velocity = base.velocity;
                        e.lengthSteps = base.lengthSteps;
                        break;
                    }
                }
            }
        }

        out.push_back({ variantEngine.makeSeedCode(120, laneCount, scaleIndex), variantEngine.getSeed(), std::move(next) });
    }
    return out;
}

juce::String SeedEngine::makeSeedCode(int tempo, int lanes, int scaleIndex) const
{
    static const char* names[4] = { "MIN", "MAJ", "DOR", "PHR" };
    return juce::String::formatted("RIFT-%06u-%s-%d-%dL", seed, names[juce::jlimit(0, 3, scaleIndex)], tempo, lanes);
}

uint32_t SeedEngine::mutate(uint32_t amount) const noexcept
{
    auto s = seed ^ amount ^ 0xA53C9E11u;
    xorshift(s);
    return s;
}
}
