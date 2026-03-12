#pragma once
#include <JuceHeader.h>

namespace riftwave {

struct PerformanceModeState
{
    bool performanceMode = false;
    bool beautyMode = true;
    int targetFps = 60;
    int particleCap = 2048;
};

class PerformanceModeController
{
public:
    void setPerformanceMode (bool enabled)
    {
        state.performanceMode = enabled;
        state.beautyMode = ! enabled;
        state.targetFps = enabled ? 30 : 60;
        state.particleCap = enabled ? 768 : 2048;
    }

    const PerformanceModeState& get() const noexcept { return state; }

private:
    PerformanceModeState state;
};

} // namespace riftwave
