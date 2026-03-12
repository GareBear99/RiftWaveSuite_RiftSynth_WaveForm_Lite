#pragma once
#include <JuceHeader.h>

namespace riftwave {
struct TransportSnapshot {
    bool isPlaying = false;
    bool isRecording = false;
    double bpm = 120.0;
    double ppq = 0.0;
    int64_t samplePosition = 0;
    int numerator = 4;
    int denominator = 4;
};

class TransportState {
public:
    void updateFrom(const juce::AudioPlayHead* playHead)
    {
        TransportSnapshot next;
        if (playHead != nullptr)
        {
            if (auto pos = playHead->getPosition())
            {
                next.isPlaying = pos->getIsPlaying();
                next.isRecording = pos->getIsRecording();
                if (auto v = pos->getBpm()) next.bpm = *v;
                if (auto v = pos->getPpqPosition()) next.ppq = *v;
                if (auto v = pos->getTimeSignature())
                {
                    next.numerator = v->numerator;
                    next.denominator = v->denominator;
                }
                if (auto v = pos->getTimeInSamples()) next.samplePosition = *v;
            }
        }
        current = next;
    }

    const TransportSnapshot& get() const noexcept { return current; }
private:
    TransportSnapshot current;
};
}
