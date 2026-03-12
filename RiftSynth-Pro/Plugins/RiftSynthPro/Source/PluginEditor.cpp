#include "PluginEditor.h"

RiftSynthProEditor::RiftSynthProEditor(RiftSynthProProcessor& p) : AudioProcessorEditor(&p), processor(p)
{
    setLookAndFeel(&lnf);
    setSize(1240, 760);

    for (auto* s : { &seed, &lanes, &cutoff, &gain, &mutateAmount })
    {
        addAndMakeVisible(*s);
        riftwave::styleRotary(*s);
    }
    for (auto* c : { &wave, &oversampling })
        addAndMakeVisible(*c);

    wave.addItemList({ "Sine", "Saw", "Square", "Triangle" }, 1);
    oversampling.addItemList({ "1x", "2x", "4x" }, 1);

    addAndMakeVisible(seedStrip);
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(midiDrag);
    addAndMakeVisible(variations);
    addAndMakeVisible(debugPanel);
    addAndMakeVisible(hostSync);
    addAndMakeVisible(lockBass);
    addAndMakeVisible(lockHarmony);
    addAndMakeVisible(uiSfx);

    variations.setMultiLine(true);
    variations.setReadOnly(true);
    debugPanel.setMultiLine(true);
    debugPanel.setReadOnly(true);
    debugPanel.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff0e1523));
    debugPanel.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff35507e));

    refreshPresetList();
    seedStrip.setStatusText("Ready");

    seedA = riftwave::bind(processor.apvts, "seed", seed);
    lanesA = riftwave::bind(processor.apvts, "lanes", lanes);
    cutoffA = riftwave::bind(processor.apvts, "cutoff", cutoff);
    gainA = riftwave::bind(processor.apvts, "gain", gain);
    mutateA = riftwave::bind(processor.apvts, "mutateAmount", mutateAmount);
    waveA = riftwave::bind(processor.apvts, "wave", wave);
    oversamplingA = riftwave::bind(processor.apvts, "oversampling", oversampling);

    hostSyncA = riftwave::bind(processor.apvts, "hostSync", hostSync);
    lockBassA = riftwave::bind(processor.apvts, "lockBass", lockBass);
    lockHarmonyA = riftwave::bind(processor.apvts, "lockHarmony", lockHarmony);
    uiSfxA = riftwave::bind(processor.apvts, "uiSfx", uiSfx);

    seedStrip.onMutate8 = [this]
    {
        processor.rebuildPattern();
        auto preview = processor.buildVariationPreview();
        variations.setText(preview.joinIntoString("\n"), juce::dontSendNotification);
        seedStrip.setStatusText("Generated 8 variations");
    };
    seedStrip.onRebuild = [this] { processor.rebuildPattern(); seedStrip.setStatusText("Pattern rebuilt"); };
    seedStrip.onSavePreset = [this]
    {
        processor.saveUserPreset("User_" + juce::Time::getCurrentTime().formatted("%Y%m%d_%H%M%S"));
        refreshPresetList();
        seedStrip.setStatusText("Preset saved");
    };

    presetBrowser.onPresetChosen = [this](const juce::String& name)
    {
        if (name.isNotEmpty())
            processor.loadUserPreset(name);
    };

    midiDrag.setFileBuilder([this] { return processor.exportMidiPreviewFile(); });
    startTimerHz(8);
}

void RiftSynthProEditor::refreshPresetList()
{
    auto items = riftwave::PresetBank::makeRiftSynthBank();
    items.addArray(processor.getUserPresetNames());
    presetBrowser.setItems(items);
}

void RiftSynthProEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff050912));
    g.setColour(juce::Colour(0xff00ffb7));
    g.drawFittedText("RiftSynth Pro", getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
}

void RiftSynthProEditor::timerCallback()
{
    seedStrip.setSeedCode(processor.getSeedCode());
    variations.setText(processor.buildVariationPreview().joinIntoString("\n"), juce::dontSendNotification);

    const auto snap = processor.getDebugSnapshot();
    juce::String dbg;
    dbg << "BPM: " << juce::String(snap.bpm, 2) << "\n";
    dbg << "Playing: " << (snap.hostPlaying ? "yes" : "no") << "\n";
    dbg << "Step: " << snap.currentStep << "\n";
    dbg << "Seed: " << snap.seedCode << "\n";
    dbg << "UI Cue: " << snap.lastUiCue << "\n";
    debugPanel.setText(dbg, juce::dontSendNotification);
}

void RiftSynthProEditor::resized()
{
    auto area = getLocalBounds().reduced(12);
    seedStrip.setBounds(area.removeFromTop(42));

    auto bottom = area.removeFromBottom(220);
    for (auto* s : { &seed, &lanes, &cutoff, &gain, &mutateAmount })
        s->setBounds(bottom.removeFromLeft(100).reduced(4));

    auto comboArea = bottom.removeFromLeft(220);
    wave.setBounds(comboArea.removeFromTop(30).reduced(4));
    oversampling.setBounds(comboArea.removeFromTop(30).reduced(4));

    auto toggles = bottom.removeFromLeft(200);
    hostSync.setBounds(toggles.removeFromTop(28));
    lockBass.setBounds(toggles.removeFromTop(28));
    lockHarmony.setBounds(toggles.removeFromTop(28));
    uiSfx.setBounds(toggles.removeFromTop(28));

    auto right = area.removeFromRight(390);
    presetBrowser.setBounds(right.removeFromTop(120));
    midiDrag.setBounds(right.removeFromTop(46).reduced(4));
    variations.setBounds(right.removeFromTop(220).reduced(4));
    debugPanel.setBounds(right.reduced(4));
}
