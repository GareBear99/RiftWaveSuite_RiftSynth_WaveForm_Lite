#include "PluginEditor.h"

WaveFormRiftSynthLiteEditor::WaveFormRiftSynthLiteEditor(WaveFormRiftSynthLiteProcessor& p)
: AudioProcessorEditor(&p), processor(p), analyzer(processor.analyzer)
{
    setLookAndFeel(&lnf);
    setSize(980, 640);
    addAndMakeVisible(analyzer);
    addAndMakeVisible(seedStrip);
    addAndMakeVisible(debugPanel);
    addAndMakeVisible(uiSfx);
    for (auto* s : { &seed, &gain })
    {
        addAndMakeVisible(*s);
        riftwave::styleRotary(*s);
    }
    debugPanel.setMultiLine(true);
    debugPanel.setReadOnly(true);
    seedA = riftwave::bind(processor.apvts, "seed", seed);
    gainA = riftwave::bind(processor.apvts, "gain", gain);
    uiSfxA = riftwave::bind(processor.apvts, "uiSfx", uiSfx);
    startTimerHz(8);
}

void WaveFormRiftSynthLiteEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff050912));
    g.setColour(juce::Colour(0xff8ef5ff));
    g.drawFittedText("WaveForm / RiftSynth Lite", getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
}

void WaveFormRiftSynthLiteEditor::timerCallback()
{
    auto snap = processor.getDebugSnapshot();
    juce::String dbg;
    dbg << "Peak: " << juce::String(snap.peak, 3) << "\n";
    dbg << "RMS: " << juce::String(snap.rms, 3) << "\n";
    dbg << "Seed: " << snap.seedCode << "\n";
    debugPanel.setText(dbg, juce::dontSendNotification);
    seedStrip.setSeedCode(snap.seedCode);
}

void WaveFormRiftSynthLiteEditor::resized()
{
    auto area = getLocalBounds().reduced(12);
    seedStrip.setBounds(area.removeFromTop(42));
    auto footer = area.removeFromBottom(180);
    seed.setBounds(footer.removeFromLeft(110).reduced(4));
    gain.setBounds(footer.removeFromLeft(110).reduced(4));
    uiSfx.setBounds(footer.removeFromLeft(100).removeFromTop(28));
    debugPanel.setBounds(footer.reduced(4));
    analyzer.setBounds(area);
}
