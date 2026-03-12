# Host Validation Matrix

| Host | Format | Focus |
|---|---|---|
| Logic Pro | AU | validation, recall, automation |
| Ableton Live | AU/VST3 | transport sync, timing, scene smoothness |
| Reaper | AU/VST3 | stress, open/close cycles, offline render |
| FL Studio | VST3 | scan compatibility, GUI scaling, synth timing |

## Mandatory cases
- load project -> save -> reopen
- switch presets during playback
- automate gain / cutoff / mix / scene
- stop/start transport repeatedly
- resize editor mid-playback
- change sample rate 44.1/48/96 kHz
