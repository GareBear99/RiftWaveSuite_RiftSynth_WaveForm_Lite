#pragma once
#include <JuceHeader.h>

namespace riftwave {

class WavPreviewExporter
{
public:
    static bool writeTempPreview (const juce::String& productName,
                                  const juce::AudioBuffer<float>& audio,
                                  double sampleRate,
                                  juce::File& outFile)
    {
        auto dir = juce::File::getSpecialLocation (juce::File::tempDirectory)
                       .getChildFile ("RiftWaveSuitePreviews");
        dir.createDirectory();
        outFile = dir.getNonexistentChildFile (productName + "_Preview", ".wav");

        juce::WavAudioFormat format;
        if (auto stream = outFile.createOutputStream())
        {
            if (auto writer = std::unique_ptr<juce::AudioFormatWriter> (format.createWriterFor (stream.get(), sampleRate,
                                                                                                 (unsigned int) audio.getNumChannels(),
                                                                                                 24, {}, 0)))
            {
                stream.release();
                writer->writeFromAudioSampleBuffer (audio, 0, audio.getNumSamples());
                return true;
            }
        }

        return false;
    }
};

} // namespace riftwave
