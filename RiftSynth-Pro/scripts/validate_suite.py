from pathlib import Path
import json, sys

root = Path(__file__).resolve().parents[1]
required = [
    root/'Plugins'/'WaveFormPro'/'Source'/'PluginProcessor.cpp',
    root/'Plugins'/'RiftSynthPro'/'Source'/'PluginProcessor.cpp',
    root/'Plugins'/'WaveFormRiftSynthLite'/'Source'/'PluginProcessor.cpp',
    root/'Shared'/'Core'/'SharedParameters.h',
    root/'Shared'/'Core'/'ProductIdentity.h',
    root/'scripts'/'host_validation_matrix.json',
    root/'docs'/'RELEASE_CHECKLIST_V8.md',
]
missing = [str(p.relative_to(root)) for p in required if not p.exists()]
if missing:
    print('Missing required paths:')
    print('
'.join(missing))
    sys.exit(1)

host_matrix = json.loads((root/'scripts'/'host_validation_matrix.json').read_text())
for plugin_dir in ['WaveFormPro', 'RiftSynthPro', 'WaveFormRiftSynthLite']:
    presets = list((root/'Plugins'/plugin_dir/'Presets').glob('*.rwspreset'))
    if not presets:
        print(f'No presets found for {plugin_dir}')
        sys.exit(1)

identity_text = (root/'Shared'/'Core'/'ProductIdentity.h').read_text()
for token in ['com.garebearaudio.riftwave.lite', 'com.garebearaudio.riftwave.waveformpro', 'com.garebearaudio.riftwave.riftsynthpro']:
    if token not in identity_text:
        print(f'Missing bundle id token: {token}')
        sys.exit(1)

print('RiftWaveSuite structure OK')
print('Hosts:', ', '.join(host_matrix['hosts']))
print('Unique identity tokens verified')

# v10 release checks
for rel in ['release/installer_manifest.json', 'release/artifact_targets.json', 'Shared/Core/SuiteVersion.h', 'Shared/Core/ReleaseIdentity.h']:
    p = root / rel
    if not p.exists():
        failures.append(f'missing v10 release asset: {rel}')
