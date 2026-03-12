#!/usr/bin/env python3
from pathlib import Path
import json
import random

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "Generated" / "SeedBanks"
OUT.mkdir(parents=True, exist_ok=True)

banks = {
    "RiftSynthPro_Showcase.json": {
        "product": "RiftSynthPro",
        "seeds": []
    },
    "WaveFormRiftSynthLite_Showcase.json": {
        "product": "WaveFormRiftSynthLite",
        "seeds": []
    },
}

rng = random.Random(99173)
for payload in banks.values():
    for _ in range(16):
        payload["seeds"].append({
            "seed": rng.randint(10000, 999999),
            "tempo": rng.choice([90, 100, 110, 120, 128, 140]),
            "scale": rng.choice(["Minor", "Dorian", "Phrygian", "Pentatonic"]),
            "lanes": rng.choice([1, 2, 3, 4]),
        })

for filename, payload in banks.items():
    with open(OUT / filename, "w", encoding="utf-8") as f:
        json.dump(payload, f, indent=2)

print(f"Generated seed banks in {OUT}")
