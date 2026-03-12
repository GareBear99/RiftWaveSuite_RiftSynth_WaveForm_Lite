    #!/usr/bin/env python3
    import sys
    from pathlib import Path

    template = """# Failure Triage

- Host:
- Plugin:
- Format:
- Build:
- Repro steps:
- Expected:
- Actual:
- Suspected layer: build / DSP / UI / transport / preset / export
"""

    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('failure_triage.md')
    out.write_text(template)
    print(f'Wrote {out}')
