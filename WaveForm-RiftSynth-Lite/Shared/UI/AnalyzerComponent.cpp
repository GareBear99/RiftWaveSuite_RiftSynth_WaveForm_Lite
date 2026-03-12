#include "AnalyzerComponent.h"

namespace riftwave {
void AnalyzerComponent::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.fillAll(juce::Colour(0xff07131d));
    g.setColour(juce::Colour(0x8027d7ff));
    g.drawRoundedRectangle(area.reduced(1.0f), 12.0f, 1.0f);

    const auto s = state.getSnapshot();
    const auto topBar = getLocalBounds().removeFromTop(24);
    g.setColour(juce::Colour(0xffd7f8ff));
    g.drawText("PK " + juce::String(juce::jmax(s.peakL, s.peakR), 2)
               + "  HOLD " + juce::String(s.peakHold, 2)
               + "  TR " + juce::String(s.transient, 2)
               + "  W " + juce::String(s.stereoWidth, 2),
               topBar, juce::Justification::centredLeft);

    auto graphArea = area.reduced(8.0f).withTrimmedTop(26.0f);
    juce::Path p;
    for (size_t i = 0; i < s.spectrum.size(); ++i)
    {
        const auto x = juce::jmap((float) i, 0.0f, (float) s.spectrum.size() - 1.0f, graphArea.getX(), graphArea.getRight());
        const auto y = graphArea.getBottom() - s.spectrum[i] * (graphArea.getHeight() - 12.0f);
        if (i == 0) p.startNewSubPath(x, y); else p.lineTo(x, y);
    }
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xff00ffb7), graphArea.getBottomLeft(),
                                           juce::Colour(0xff3be0ff), graphArea.getTopRight(), false));
    g.strokePath(p, juce::PathStrokeType(2.2f));

    auto bars = graphArea.removeFromBottom(34.0f);
    auto drawMeter = [&](juce::Rectangle<float> r, float value, juce::Colour c, const juce::String& label)
    {
        g.setColour(juce::Colour(0x221ffffff));
        g.fillRoundedRectangle(r, 5.0f);
        auto fill = r.withWidth(r.getWidth() * juce::jlimit(0.0f, 1.0f, value));
        g.setColour(c);
        g.fillRoundedRectangle(fill, 5.0f);
        g.setColour(juce::Colour(0xffd7f8ff));
        g.drawText(label, r.toNearestInt(), juce::Justification::centredLeft);
    };
    auto meterW = bars.getWidth() / 3.0f - 6.0f;
    drawMeter({ bars.getX(), bars.getY(), meterW, bars.getHeight() }, s.lowEnergy, juce::Colour(0xff00ffb7), "LOW");
    drawMeter({ bars.getX() + meterW + 9.0f, bars.getY(), meterW, bars.getHeight() }, s.midEnergy, juce::Colour(0xff3be0ff), "MID");
    drawMeter({ bars.getRight() - meterW, bars.getY(), meterW, bars.getHeight() }, s.highEnergy, juce::Colour(0xffff2fa4), "HIGH");
}
}
