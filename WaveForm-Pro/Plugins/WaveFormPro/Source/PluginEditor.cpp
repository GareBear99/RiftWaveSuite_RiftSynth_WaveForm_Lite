#include "PluginEditor.h"

WaveFormProEditor::WaveFormProEditor(WaveFormProProcessor& p)
: AudioProcessorEditor(&p), processor(p), scene(processor.analyzer, mode)
{
    setLookAndFeel(&lnf);
    setSize(1160, 740);

    for (auto* s : { &gain, &mix, &smooth, &density, &width, &peakHold })
    {
        addAndMakeVisible(*s);
        riftwave::styleRotary(*s);
    }

    addAndMakeVisible(scene);
    addAndMakeVisible(sceneBox);
    addAndMakeVisible(beautyMode);
    addAndMakeVisible(uiSfx);
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(debugPanel);

    debugPanel.setMultiLine(true);
    debugPanel.setReadOnly(true);
    debugPanel.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff0e1523));
    debugPanel.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff35507e));

    sceneBox.addItemList({ "Ribbon", "Orbit", "Pulse", "Tunnel", "Ladder" }, 1);
    sceneBox.onChange = [this]
    {
        mode = static_cast<riftwave::SceneMode>(juce::jlimit(0, 4, sceneBox.getSelectedItemIndex()));
        repaint();
        processor.uiSfx.trigger(riftwave::UiSfxEngine::Cue::Spark);
    };

    refreshPresetList();

    gainA = riftwave::bind(processor.apvts, "gain", gain);
    mixA = riftwave::bind(processor.apvts, "mix", mix);
    smoothA = riftwave::bind(processor.apvts, "smooth", smooth);
    densityA = riftwave::bind(processor.apvts, "density", density);
    widthA = riftwave::bind(processor.apvts, "width", width);
    peakHoldA = riftwave::bind(processor.apvts, "peakHold", peakHold);
    sceneA = riftwave::bind(processor.apvts, "scene", sceneBox);
    beautyA = riftwave::bind(processor.apvts, "beautyMode", beautyMode);
    uiSfxA = riftwave::bind(processor.apvts, "uiSfx", uiSfx);

    presetBrowser.onPresetChosen = [this](const juce::String& name)
    {
        if (name.isNotEmpty())
            processor.loadUserPreset(name);
    };

    startTimerHz(12);
}

void WaveFormProEditor::refreshPresetList()
{
    auto items = riftwave::PresetBank::makeWaveFormBank();
    items.addArray(processor.getUserPresetNames());
    presetBrowser.setItems(items);
}

void WaveFormProEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff050912));
    auto top = getLocalBounds().removeFromTop(40);
    g.setColour(juce::Colour(0xff00ffb7));
    g.drawFittedText("WaveForm Pro", top, juce::Justification::centred, 1);
}

void WaveFormProEditor::timerCallback()
{
    const auto snap = processor.getDebugSnapshot();
    juce::String dbg;
    dbg << "BPM: " << juce::String(snap.bpm, 2) << "\n";
    dbg << "Playing: " << (snap.hostPlaying ? "yes" : "no") << "\n";
    dbg << "Peak: " << juce::String(snap.peak, 3) << "\n";
    dbg << "RMS: " << juce::String(snap.rms, 3) << "\n";
    dbg << "UI Cue: " << snap.lastUiCue << "\n";
    debugPanel.setText(dbg, juce::dontSendNotification);
}

void WaveFormProEditor::resized()
{
    auto area = getLocalBounds().reduced(12);
    auto top = area.removeFromTop(44);
    sceneBox.setBounds(top.removeFromRight(180).reduced(4));
    beautyMode.setBounds(top.removeFromRight(110).reduced(4));
    uiSfx.setBounds(top.removeFromRight(110).reduced(4));

    auto bottom = area.removeFromBottom(200);
    auto leftRack = bottom.removeFromLeft(620);
    auto rightRack = bottom;

    auto slot = [&](juce::Slider& s)
    {
        s.setBounds(leftRack.removeFromLeft(100).reduced(4));
    };
    slot(gain); slot(mix); slot(smooth); slot(density); slot(width); slot(peakHold);

    presetBrowser.setBounds(rightRack.removeFromTop(120).reduced(4));
    debugPanel.setBounds(rightRack.reduced(4));
    scene.setBounds(area);
}
