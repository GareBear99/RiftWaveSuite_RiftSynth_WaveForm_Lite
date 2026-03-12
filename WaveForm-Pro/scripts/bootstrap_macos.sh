#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
JUCE_DIR="${JUCE_DIR:-$HOME/SDKs/JUCE}"
BUILD_DIR="${BUILD_DIR:-$ROOT/build-macos}"

echo "[RiftWaveSuite] root: $ROOT"
echo "[RiftWaveSuite] JUCE_DIR: $JUCE_DIR"
[ -d "$JUCE_DIR" ] || { echo "Missing JUCE checkout at $JUCE_DIR"; exit 1; }
cmake -S "$ROOT" -B "$BUILD_DIR" -DJUCE_DIR="$JUCE_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --config Release -j 8

echo "Build complete. Validate AU and VST3 in Logic, Reaper, and Live before release."
