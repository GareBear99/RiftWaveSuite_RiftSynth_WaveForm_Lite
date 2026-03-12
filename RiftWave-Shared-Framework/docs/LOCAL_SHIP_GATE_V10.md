# Local Ship Gate v10

Release is allowed only after all of the following are true:
1. CMake config succeeds locally against a real JUCE checkout.
2. All 3 products build in Debug and Release.
3. Logic, Ableton, Reaper, and FL Studio smoke tests pass.
4. Project save/load recall passes.
5. Preset load/save passes for each product.
6. WaveForm Pro visual scenes stay responsive without UI stalls.
7. RiftSynth Pro transport start/stop and loop-region sync pass.
8. Bundle IDs are unique and stable.
9. macOS signing + notarization complete.
10. Factory presets are frozen and tagged.
