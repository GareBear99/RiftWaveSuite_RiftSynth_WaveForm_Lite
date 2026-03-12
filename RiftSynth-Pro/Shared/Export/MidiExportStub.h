#pragma once
#include <JuceHeader.h>
#include "../Seed/SeedEngine.h"

namespace riftwave {
class MidiExportStub
{
public:
    static juce::MidiMessageSequence makeSequence(const std::vector<LaneEvent>& events, double bpm, int ppq = 960)
    {
        juce::ignoreUnused(ppq);
        juce::MidiMessageSequence seq;
        const auto secondsPerBeat = 60.0 / juce::jmax(1.0, bpm);
        const auto stepSeconds = secondsPerBeat / 4.0;
        for (const auto& e : events)
        {
            const auto noteOnTime = e.startStep * stepSeconds;
            const auto noteOffTime = (e.startStep + juce::jmax(1, e.lengthSteps)) * stepSeconds;
            seq.addEvent(juce::MidiMessage::noteOn(1, e.midiNote, (juce::uint8) juce::roundToInt(e.velocity * 127.0f)), noteOnTime);
            seq.addEvent(juce::MidiMessage::noteOff(1, e.midiNote), noteOffTime);
        }
        seq.updateMatchedPairs();
        return seq;
    }

    static std::unique_ptr<juce::MidiFile> makeMidiFile(const std::vector<LaneEvent>& events, double bpm, int tpq = 960)
    {
        auto midi = std::make_unique<juce::MidiFile>();
        midi->setTicksPerQuarterNote(tpq);
        auto seq = makeSequence(events, bpm, tpq);
        midi->addTrack(seq);
        return midi;
    }

    static bool writeMidiFile(const juce::File& file, const std::vector<LaneEvent>& events, double bpm, int tpq = 960)
    {
        if (! file.getParentDirectory().createDirectory())
            return false;
        if (auto stream = file.createOutputStream())
        {
            auto midi = makeMidiFile(events, bpm, tpq);
            midi->writeTo(*stream, 1);
            return true;
        }
        return false;
    }
};
}
