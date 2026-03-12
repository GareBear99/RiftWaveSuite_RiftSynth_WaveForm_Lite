# RiftWaveSuite 

WaveForm / RiftSynth Lite

Repo: WaveForm-RiftSynth-Lite

# WaveForm / RiftSynth Lite

Free hybrid audio plugin combining **real-time visualization** and **seed-based generative synthesis**.

WaveForm / RiftSynth Lite serves as the entry point to the RiftWave plugin ecosystem.

It includes lightweight versions of two plugins:

• WaveForm Pro  
• RiftSynth Pro  

---

# Features

## WaveForm Lite

Real-time audio visualization.

Includes:

• waveform display  
• spectrum analyzer  
• stereo scope  
• energy visualization  

Designed to be lightweight and responsive.

---

## RiftSynth Lite

Deterministic seed-based pattern generator.

Includes:

• single synth lane  
• seed-based pattern generation  
• basic oscillator engine  
• preview playback  

Same seed always produces the same musical result.

---

# Limitations (Compared to Pro Versions)

WaveForm Lite

• limited visual scenes  
• fewer customization options  

RiftSynth Lite

• single synth lane  
• limited synth engine  
• no MIDI export  
• simplified preset system  

---

# Upgrade Path

WaveForm Pro

Advanced audio visualization including:

• multiple visual scenes  
• GPU rendering  
• performance mode  

RiftSynth Pro

Full generative synth including:

• multi-lane sequencing  
• seed mutation tools  
• MIDI export  
• preset browser  

---

# Architecture

Built on top of the **RiftWave Shared Framework**.

Uses the following modules:

• Audio analyzer  
• Seed engine  
• Synth voice  
• Step sequencer  
• Preset system  

---

# Build

Requirements:

• JUCE  
• CMake  
• modern C++ compiler  

Typical workflow:


bootstrap project
generate CMake build
compile plugin


---

# Status

Free plugin within the RiftWave ecosystem.

Designed to provide a lightweight introduction to the full plugin suite.

---

# License

MIT License
3️⃣ WaveForm Pro

Repo: WaveForm-Pro

# WaveForm Pro

Advanced real-time audio visualization plugin.

WaveForm Pro converts incoming audio into dynamic visual motion, useful for production analysis and visual performance.

---

# Features

Real-time audio analysis including:

• FFT spectrum  
• waveform ribbons  
• stereo field scope  
• transient detection  
• energy band visualization  

---

# Visual Scenes

WaveForm Pro supports multiple visual modes.

Examples include:

• ribbon scope  
• spectral ladder  
• stereo tunnel  
• orbit field  
• pulse wall  

Scenes react dynamically to incoming audio.

---

# Performance Modes

Two rendering modes are available.

Performance Mode

• optimized CPU usage  
• minimal GPU load  

Beauty Mode

• enhanced visual detail  
• richer particle effects  

---

# Applications

WaveForm Pro is useful for:

• mixing analysis  
• mastering monitoring  
• live visuals  
• streaming overlays  
• music visualization  

---

# Architecture

WaveForm Pro uses systems from the **RiftWave Shared Framework**:

• audio analyzer  
• visual renderer  
• parameter system  
• preset system  
• theme engine  

---

# Build

Requirements:

• JUCE  
• CMake  
• modern C++ compiler  

Typical workflow:


bootstrap project
generate CMake build
compile plugin


---

# Status

Part of the RiftWave plugin suite.

WaveForm Pro focuses on **audio visualization and analysis**.

---

# License

MIT License
4️⃣ RiftSynth Pro

Repo: RiftSynth-Pro

# RiftSynth Pro

Deterministic generative synthesizer powered by seed-based pattern generation.

RiftSynth Pro allows producers to generate repeatable musical ideas using a single seed value.

Same seed → same music.

---

# Core Concept

Music generation is based on:

• seed value  
• tempo  
• scale  
• pattern rules  

Example:

Seed: 482211  
Scale: Minor  
Tempo: 128  

The same configuration always produces identical results.

---

# Features

## Multi-Lane Sequencing

Supports multiple pattern lanes.

Typical uses include:

• bass  
• chords  
• melody  
• percussion  

---

## Seed System

Seed controls the entire pattern.

Tools include:

• mutate seed  
• generate variations  
• lock pattern sections  
• seed favorites  

---

## Synth Engine

Includes:

• band-limited oscillators  
• sub oscillator  
• envelope control  
• filter shaping  
• parameter smoothing  

---

## Export

Supports:

• MIDI export  
• seed sharing  
• preset storage  

---

# Workflow

Typical workflow:

1. generate seed  
2. mutate variations  
3. select favorite  
4. export MIDI  
5. build track around pattern  

---

# Architecture

RiftSynth Pro uses modules from the **RiftWave Shared Framework**:

• seed engine  
• synth voice  
• step sequencer  
• preset system  
• export helpers  

---

# Build

Requirements:

• JUCE  
• CMake  
• modern C++ compiler  

Typical workflow:


bootstrap project
generate CMake build
compile plugin


---

# Status

Flagship generative instrument in the RiftWave plugin suite.

---

# License

MIT License

# RiftWave Shared Framework

Core engine behind the **RiftWave audio plugin suite**.

RiftWave is a modular DSP architecture used to power multiple audio plugins while sharing a single internal framework.

This repository contains the shared DSP, analyzer, sequencing, preset, UI, and export systems used by:

• WaveForm / RiftSynth Lite (free)  
• WaveForm Pro  
• RiftSynth Pro  

Instead of duplicating code across repositories, these plugins build on top of a common engine layer.

---

# Overview

RiftWave provides a reusable framework for building modern audio plugins.

The framework handles the common systems required by audio tools:

• DSP synthesis  
• real-time audio analysis  
• generative sequencing  
• preset management  
• UI infrastructure  
• export utilities  

All plugins in the RiftWave suite build on this shared foundation.

---

# Architecture


DAW Host
│
├─ WaveForm / RiftSynth Lite
├─ WaveForm Pro
├─ RiftSynth Pro
│
└─ RiftWave Shared Framework
├─ DSP Engine
├─ Audio Analyzer
├─ Seed Engine
├─ Sequencer
├─ Preset System
└─ UI Systems


This architecture ensures:

• consistent behavior across plugins  
• less duplicated code  
• easier maintenance  
• faster development  

---

# Core Systems

## DSP

Low-level audio building blocks used by the synth engine.

Includes:

• band-limited oscillator shells  
• oversampling engine scaffolding  
• parameter smoothing helpers  
• voice lifecycle helpers  

---

## Audio Analysis

Used by WaveForm plugins.

Provides real-time signal analysis including:

• FFT spectrum analysis  
• RMS and peak detection  
• stereo correlation  
• energy band extraction  
• transient detection  

---

## Seed Engine

Used by RiftSynth.

Deterministic generation system that produces repeatable musical output.

Features:

• deterministic random generator  
• seed-based pattern generation  
• seed mutation and variation tools  
• seed locking mechanisms  

Same seed → same musical output.

---

## Sequencing

Transport-aware step scheduler.

Supports:

• host BPM synchronization  
• sample-accurate timing  
• pattern playback  
• note lifecycle management  

---

## Preset System

Shared preset architecture used across the suite.

Includes:

• preset codec  
• JSON serialization helpers  
• preset indexing  
• tagged preset lookup  

---

## UI Systems

Reusable interface infrastructure.

Includes:

• UI SFX engine  
• theme system  
• parameter attachment helpers  
• debug snapshot tools  

---

## Export Tools

Shared export helpers.

Includes scaffolding for:

• MIDI export  
• WAV preview export  
• seed sharing  

---

# Build

Plugins using this framework rely on:

• JUCE  
• CMake  
• modern C++ compiler  

Typical workflow:


git clone
run bootstrap script
generate CMake project
build plugin target


---

# Projects Using This Framework

WaveForm / RiftSynth Lite  
Free hybrid visualizer and generative synth.

WaveForm Pro  
Advanced real-time audio visualization plugin.

RiftSynth Pro  
Seed-driven generative synthesizer.

---

# License

MIT License

---

# Status

This framework is under active development as part of the **RiftWave plugin suite**.


