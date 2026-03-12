# QA Checklist

## Build / validation
- [ ] JUCE path configured
- [ ] macOS Debug build
- [ ] macOS Release build
- [ ] AU scan succeeds
- [ ] VST3 scan succeeds

## Product identity / conflict gate
- [ ] unique bundle IDs verified
- [ ] unique 4-char plugin codes verified
- [ ] no collision with FreeEQ8 / Maid plugins

## Audio correctness
- [ ] no denormal spikes
- [ ] no stuck notes
- [ ] note releases sound correct
- [ ] sample-rate changes safe
- [ ] automation sweeps do not zipper badly

## UI correctness
- [ ] attachments recall correctly
- [ ] preset browser selection stable
- [ ] no repaint runaway
- [ ] resize safe
- [ ] seed strip updates correctly

## Host behavior
- [ ] Reaper open/save/reopen
- [ ] Ableton transport start/stop
- [ ] Logic validation
- [ ] FL Studio scan and playback
- [ ] offline bounce works
