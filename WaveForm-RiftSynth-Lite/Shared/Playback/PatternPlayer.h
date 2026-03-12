#pragma once
#include <JuceHeader.h>
#include "../Seed/SeedEngine.h"
#include <vector>
#include <algorithm>

namespace riftwave {
class PatternPlayer
{
public:
    void setPattern(std::vector<LaneEvent> newPattern, int newStepsPerBar)
    {
        pattern = std::move(newPattern);
        stepsPerBar = juce::jmax(1, newStepsPerBar);
        reset();
    }

    void reset()
    {
        active.clear();
    }

    template <typename NoteOnFn, typename NoteOffFn>
    void handleStep(int step, int sampleOffset, NoteOnFn&& noteOn, NoteOffFn&& noteOff)
    {
        const int normalizedStep = ((step % stepsPerBar) + stepsPerBar) % stepsPerBar;

        for (auto it = active.begin(); it != active.end();)
        {
            if (it->endStep == normalizedStep)
            {
                noteOff(it->midiNote, sampleOffset);
                it = active.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (const auto& e : pattern)
        {
            if (e.startStep != normalizedStep)
                continue;

            noteOn(e.midiNote, e.velocity, sampleOffset);
            ActiveNote a;
            a.midiNote = e.midiNote;
            a.endStep = (e.startStep + juce::jmax(1, e.lengthSteps)) % stepsPerBar;
            active.push_back(a);
        }
    }

    template <typename NoteOffFn>
    void allNotesOff(NoteOffFn&& noteOff)
    {
        for (const auto& n : active)
            noteOff(n.midiNote, 0);
        active.clear();
    }

    const std::vector<LaneEvent>& getPattern() const noexcept { return pattern; }

private:
    struct ActiveNote
    {
        int midiNote = -1;
        int endStep = 0;
    };

    int stepsPerBar = 16;
    std::vector<LaneEvent> pattern;
    std::vector<ActiveNote> active;
};
}
