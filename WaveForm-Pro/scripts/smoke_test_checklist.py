from pathlib import Path
root = Path(__file__).resolve().parents[1]
checks = {
    'preset_banks': len(list((root/'Plugins'/'RiftSynthPro'/'Presets').glob('*.rwspreset'))) >= 5,
    'sfx_assets': len(list((root/'Assets'/'SFX').glob('*.wav'))) >= 5,
    'seed_banks': len(list((root/'Generated'/'SeedBanks').glob('*.json'))) >= 2,
}
for key, ok in checks.items():
    print(f'{key}: {"OK" if ok else "MISSING"}')
