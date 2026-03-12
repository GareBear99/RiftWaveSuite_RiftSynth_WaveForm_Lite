#pragma once
#include "ProductTypes.h"

namespace riftwave {
struct ProductConfig {
    ProductTier tier {};
    bool allowVisualScenes = false;
    bool allowSeedFavorites = false;
    bool allowMidiExport = false;
    bool allowAudioExport = false;
    int maxLanes = 1;
    int maxVisualBands = 64;
};

inline ProductConfig getProductConfig(ProductTier tier)
{
    switch (tier)
    {
        case ProductTier::Lite: return { tier, false, false, false, false, 1, 64 };
        case ProductTier::WaveFormPro: return { tier, true, false, false, false, 1, 128 };
        case ProductTier::RiftSynthPro: return { tier, false, true, true, true, 4, 64 };
    }
    return { tier, false, false, false, false, 1, 64 };
}
}
