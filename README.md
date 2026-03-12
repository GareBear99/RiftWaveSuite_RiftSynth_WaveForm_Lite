# RiftWaveSuite Shared Framework

Core framework used by the RiftWave audio plugin suite.

This repository contains the shared DSP, analyzer, sequencing, preset, UI, and infrastructure code used by:

- WaveForm / RiftSynth Lite (free)
- WaveForm Pro
- RiftSynth Pro

It exists to prevent duplicated code across plugin repositories and to ensure a consistent architecture across the entire suite.

---

## Framework Modules

### DSP
Shared audio DSP building blocks.

- band-limited oscillator shells
- oversampling engine shell
- parameter smoothing
- voice lifecycle helpers

---

### Audio Analysis
Used by WaveForm plugins.

Includes scaffolding for:

- FFT analysis
- RMS and peak detection
- stereo correlation
- energy band extraction
- transient detection

---

### Seed Engine
Used by RiftSynth.

Features:

- deterministic RNG
- seed-based pattern generation
- variation generation
- seed locking and mutation hooks

---

### Sequencing
Transport-aware step scheduler.

Supports:

- host BPM sync
- sample-domain timing
- pattern playback
- note lifecycle management

---

### Preset System
Shared preset architecture.

Includes:

- preset codec
- JSON round-trip helpers
- preset indexing
- tagged preset lookup

---

### UI Systems
Shared UI components.

Includes:

- UI SFX engine
- theme bank
- parameter attachment helpers
- debug snapshot helpers

---

### Export Tools

Includes shells for:

- MIDI file export
- WAV preview export
- seed sharing

---

## Architecture


Host DAW
│
├─ Plugin (Lite / WaveForm / RiftSynth)
│
└─ RiftWave Shared Framework
├─ DSP
├─ Analyzer
├─ Seed Engine
├─ Sequencer
├─ Presets
└─ UI


This ensures:

- consistent behavior across plugins
- reduced maintenance
- faster development

---

## Build

Plugins using this framework rely on:

- JUCE
- CMake

Typical flow:


git clone
run bootstrap script
generate CMake project
build plugin target


---

## License

MIT License.

---

## Status

This framework is part of the RiftWave plugin suite development and may evolve alongside the plugin repositories.
README — WaveForm / RiftSynth Lite

Repo: WaveForm-RiftSynth-Lite

# WaveForm / RiftSynth Lite

Free hybrid plugin combining audio visualization and seed-based generative synthesis.

This plugin is the **entry point to the RiftWave ecosystem**.

It provides lightweight versions of the two main products:

- WaveForm Pro
- RiftSynth Pro

---

## Features

### WaveForm Lite

Real-time audio visualization.

Includes:

- waveform display
- spectrum analyzer
- stereo scope
- energy visualization

Designed to be lightweight and responsive.

---

### RiftSynth Lite

Deterministic seed-based pattern generator.

Includes:

- single synth lane
- seed-based pattern generation
- basic oscillator
- quick preview playback

Same seed always produces the same musical output.

---

## Limitations (vs Pro)

WaveForm Lite

- limited visual modes
- fewer customization controls

RiftSynth Lite

- single lane only
- limited synth engine
- no MIDI export
- limited preset system

---

## Upgrade Path

WaveForm Pro

Advanced audio visualization with multiple scenes, GPU rendering, and performance mode.

RiftSynth Pro

Full generative synth with:

- multi-lane sequencing
- seed mutation
- MIDI export
- preset browser

---

## Architecture

Built using the **RiftWave Shared Framework**.

Modules used:

- Analyzer
- Seed engine
- Synth voice
- Sequencer
- Preset system

---

## Build

Requirements:

- JUCE
- CMake
- modern C++ compiler

Typical build flow:


bootstrap project
generate CMake build
compile plugin


---

## Status

Free version of the RiftWave plugin suite.

Designed to be lightweight, stable, and easy to explore.

---

## License

MIT License.
README — WaveForm Pro

Repo: WaveForm-Pro

# WaveForm Pro

Advanced real-time audio visualization plugin.

WaveForm Pro transforms incoming audio into rich visual motion, making it useful for both production analysis and visual performance.

---

## Features

Real-time analysis including:

- FFT spectrum
- waveform ribbons
- stereo field scope
- transient detection
- energy band visualization

---

## Visual Scenes

WaveForm Pro supports multiple visual modes.

Examples include:

- ribbon scope
- spectral ladder
- stereo tunnel
- orbit field
- pulse wall

Scenes react dynamically to incoming audio.

---

## Performance Modes

Two render modes are available.

Performance mode

- optimized CPU usage
- minimal GPU load

Beauty mode

- enhanced visuals
- richer particle effects

---

## Applications

WaveForm Pro is useful for:

- mixing analysis
- mastering monitoring
- live visuals
- streaming overlays
- music visualization

---

## Architecture

WaveForm Pro uses modules from the RiftWave Shared Framework:

- Audio analyzer
- Visual renderer
- Parameter system
- Preset system
- UI theme engine

---

## Build

Requirements:

- JUCE
- CMake
- modern C++ compiler

Typical flow:


bootstrap project
generate CMake build
compile plugin


---

## Status

Part of the RiftWave plugin suite.

WaveForm Pro focuses entirely on **visual analysis and performance visualization**.

---

## License

MIT License.
README — RiftSynth Pro

Repo: RiftSynth-Pro

# RiftSynth Pro

Deterministic generative synthesizer powered by seed-based pattern generation.

RiftSynth Pro allows producers to generate repeatable musical ideas using a single seed value.

Same seed = same music.

---

## Core Concept

Music generation is based on:

- seed value
- tempo
- scale
- pattern rules

Example:


Seed: 482211
Scale: Minor
Tempo: 128


The output will always be identical for the same configuration.

---

## Features

### Multi-lane Sequencing

Supports multiple independent pattern lanes.

Typical uses:

- bass
- chords
- melody
- percussion

---

### Seed System

Seed controls the entire pattern.

Tools include:

- mutate seed
- generate variations
- lock sections
- seed favorites

---

### Synth Engine

Includes:

- band-limited oscillators
- sub oscillator
- envelope control
- filter shaping
- parameter smoothing

---

### Export

Supports:

- MIDI export
- seed sharing
- preset storage

---

## Workflow

Typical workflow:

1. generate seed
2. mutate variations
3. select favorite
4. export MIDI
5. build track around pattern

---

## Architecture

RiftSynth Pro uses modules from the RiftWave Shared Framework:

- seed engine
- synth voice
- step scheduler
- preset system
- export helpers

---

## Build

Requirements:

- JUCE
- CMake
- modern C++ compiler

Typical build flow:


bootstrap project
generate CMake build
compile plugin


---

## Status

Flagship generative instrument in the RiftWave plugin suite.

---

## License

MIT License.


Part of the RiftWave Audio Plugin Suite.
