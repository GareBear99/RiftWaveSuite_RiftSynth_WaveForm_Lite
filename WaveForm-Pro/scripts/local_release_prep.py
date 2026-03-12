#!/usr/bin/env python3
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
meta = json.loads((root / 'release' / 'installer_manifest.json').read_text())
print('RiftWaveSuite local release prep')
print('Version:', meta['version'])
for product in meta['products']:
    print('-', product['name'], '->', product['bundle_id'])
print('Next: run bootstrap, compile, execute host validation matrix, then sign/notarize.')
