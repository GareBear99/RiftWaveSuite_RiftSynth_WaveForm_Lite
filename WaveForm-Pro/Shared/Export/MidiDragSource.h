#pragma once
#include <JuceHeader.h>

namespace riftwave {
class MidiDragSource : public juce::Component
{
public:
    using FileBuilder = std::function<juce::File(void)>;

    void setFileBuilder(FileBuilder builder) { fileBuilder = std::move(builder); }

    void mouseDown(const juce::MouseEvent&) override
    {
        if (fileBuilder)
            stagedFile = fileBuilder();
    }

    void mouseDrag(const juce::MouseEvent&) override
    {
        if (stagedFile.existsAsFile())
        {
            juce::StringArray files;
            files.add(stagedFile.getFullPathName());
            performExternalDragDropOfFiles(files, false);
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(1.5f);
        g.setColour(juce::Colour(0xff121d2f));
        g.fillRoundedRectangle(area, 10.0f);
        g.setColour(juce::Colour(0xff00ffb7).withAlpha(0.8f));
        g.drawRoundedRectangle(area, 10.0f, 1.5f);
        g.drawFittedText("Drag MIDI", getLocalBounds(), juce::Justification::centred, 1);
    }

private:
    FileBuilder fileBuilder;
    juce::File stagedFile;
};
}
