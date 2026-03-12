from pathlib import Path
import hashlib

root = Path(__file__).resolve().parents[1]
for path in sorted(p for p in root.rglob('*') if p.is_file()):
    rel = path.relative_to(root).as_posix()
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    print(f"{digest}  {rel}")
