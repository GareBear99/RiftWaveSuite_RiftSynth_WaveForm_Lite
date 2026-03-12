#!/usr/bin/env python3
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
targets = json.loads((root / 'release' / 'artifact_targets.json').read_text())
print('Expected release artifacts:')
for item in targets['artifacts']:
    print(' -', item)
print('Packaging stub only: create signed zip/pkg after local host validation passes.')
