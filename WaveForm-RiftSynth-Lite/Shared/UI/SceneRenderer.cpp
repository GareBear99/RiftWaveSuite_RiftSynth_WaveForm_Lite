#include "SceneRenderer.h"

namespace riftwave {
void SceneRenderer::paint(juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.fillAll(juce::Colour(0xff040911));
    auto s = analyzer.getSnapshot();
    const auto center = area.getCentre();

    if (mode == SceneMode::Orbit)
    {
        g.setColour(juce::Colour(0xff00ffb7));
        for (int i = 0; i < 5; ++i)
        {
            const auto r = 20.0f + i * (24.0f + s.lowEnergy * 10.0f) + s.transient * 11.0f;
            g.drawEllipse(center.x - r, center.y - r, r * 2.0f, r * 2.0f, 1.4f);
        }
    }
    else if (mode == SceneMode::StereoTunnel)
    {
        g.setColour(juce::Colour(0xff3be0ff));
        for (int i = 0; i < 8; ++i)
        {
            auto pad = 10.0f + i * (18.0f + s.stereoWidth * 8.0f);
            g.drawRoundedRectangle(area.reduced(pad), 12.0f, 1.2f);
        }
    }
    else if (mode == SceneMode::PulseWall)
    {
        const int bars = 32;
        const auto bw = area.getWidth() / (float) bars;
        for (int i = 0; i < bars; ++i)
        {
            const auto t = (float) i / (float) (bars - 1);
            const auto idx = (int) std::floor(t * (float) (s.spectrum.size() - 1));
            const auto h = s.spectrum[(size_t) idx] * area.getHeight();
            g.setColour(juce::Colour::fromHSV(0.88f - 0.2f * t, 0.8f, 0.95f, 0.9f));
            g.fillRoundedRectangle(area.getX() + i * bw, area.getBottom() - h, bw - 3.0f, h, 4.0f);
        }
    }
    else
    {
        juce::Path p;
        for (size_t i = 0; i < s.spectrum.size(); ++i)
        {
            const auto x = juce::jmap((float) i, 0.0f, (float) s.spectrum.size() - 1.0f, area.getX(), area.getRight());
            const auto y = area.getBottom() - s.spectrum[i] * (area.getHeight() - 12.0f);
            if (i == 0) p.startNewSubPath(x, y); else p.lineTo(x, y);
        }
        g.setColour(mode == SceneMode::PulseWall ? juce::Colour(0xffff2fa4) : juce::Colour(0xff00ffb7));
        g.strokePath(p, juce::PathStrokeType(mode == SceneMode::Ribbon ? 3.5f : 2.6f));
    }

    g.setColour(juce::Colour(0x18ffffff));
    g.drawRoundedRectangle(area.reduced(1.0f), 14.0f, 1.0f);
}
}
