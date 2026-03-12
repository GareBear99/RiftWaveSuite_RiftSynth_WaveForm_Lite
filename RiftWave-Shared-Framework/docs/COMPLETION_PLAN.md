# Completion Plan

## Phase A — Compile & Link
- Point `JUCE_DIR` at local checkout.
- Generate AU/VST3 projects.
- Resolve any include/link mismatches.

## Phase B — DSP Hardening
- Replace placeholder synth path with antialiased oscillator bank.
- Finalize envelope smoothing and voice stealing.
- Validate analyzer frame cadence and render decoupling.

## Phase C — Product Hooks
- Wire preset browser to codec.
- Implement MIDI export and drag-out.
- Add seed favorites persistence.
- Add theme switching and UI SFX toggle.

## Phase D — QA
- Host scan validation.
- Save/load/reopen tests.
- Offline bounce and sample-rate matrix.
