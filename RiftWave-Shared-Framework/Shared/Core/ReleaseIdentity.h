#pragma once

namespace rws {
struct ReleaseIdentity {
    const char* suiteName;
    const char* manufacturer;
    const char* channel;
    const char* version;
};

static constexpr ReleaseIdentity kReleaseIdentity {
    "RiftWaveSuite",
    "GareBear Audio",
    "local-handoff",
    "DARPA_v10"
};
}
