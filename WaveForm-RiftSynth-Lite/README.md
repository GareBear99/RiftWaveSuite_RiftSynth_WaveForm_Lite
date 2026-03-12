# WaveForm / RiftSynth Lite

Free hybrid plugin combining lightweight reactive visualization and seeded synth features.

This drag-and-drop repo package was split from `RiftWaveSuite_DARPA_v10` so it can be uploaded to GitHub as its own repository.

## Included
- `Plugins/WaveFormRiftSynthLite/` plugin-specific source and presets
- `Shared/` common framework code required by this scaffold
- `Assets/` shared UI SFX placeholders
- `cmake/` common CMake helper
- `scripts/` compile/validation helpers
- `docs/` DARPA audit and ship-gate notes
- `release/` release manifest scaffolding

## Local build
1. Clone or drop this repo anywhere on disk.
2. Install JUCE locally.
3. Configure CMake with `-DJUCE_DIR=/path/to/JUCE`.
4. Build the target from your IDE or generator.

## Notes
- This is still a compile/validation scaffold, not a finalized commercial release binary.
- Bundle IDs and release metadata should be checked locally before shipping.
