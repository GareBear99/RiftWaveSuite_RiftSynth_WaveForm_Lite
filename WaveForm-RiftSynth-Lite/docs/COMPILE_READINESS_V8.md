# Compile Readiness V8

This pass focuses on turning the suite from a strong scaffold into a compile-first integration package.

## Goals
- lock expected JUCE and CMake floors
- provide bootstrap scripts for macOS and Windows
- expose remaining editor-side attachments needed for APVTS coverage
- make host validation a first-class artifact

## Required local environment
- JUCE 8.x checkout available locally
- CMake 3.22+
- Xcode toolchain on macOS for AU/VST3
- Visual Studio 2022 on Windows for VST3

## Compile gate
The suite is not considered shippable until all three products build in Release and survive scan/insert/open-close validation in at least Reaper and one flagship host per platform.
