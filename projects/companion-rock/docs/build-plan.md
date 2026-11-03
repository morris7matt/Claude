# Build plan

Dates assume the parts order goes in the first week of November and Christmas is **Dec 25**.
The schedule has about a week of slack at the end. Don't spend it early.

| Week | Goal | Done when |
|---|---|---|
| **Nov 3-9** | Finalize the parts list and order everything. Read the docs. | Order confirmed with delivery dates before Nov 16 |
| **Nov 10-16** | Parts arrive. Bench-test each board on a breadboard. | Serial shows `mpu=1 aht=1 drv=1` |
| **Nov 17-23** | Flash the firmware on the breadboard, tune the touch pad, volume and haptics. Start the shell design. | All four behaviors work on the bench |
| **Nov 24-30** | Print a fit-check shell, fix sizes. Mock up the pet pad and speaker grille. | Parts fit with room for wires |
| **Dec 1-7** | Final print, solder and assemble **rock #1**. | Rock #1 works on battery, closed up |
| **Dec 8-14** | Fix what rock #1 taught you, then build **rock #2**. | Both rocks assembled |
| **Dec 15-21** | Testing, repairs, charge and drop tests. Wrap. | Test checklist below passes for both |
| **Dec 22-24** | Slack, charge, final check. | |

**Do not skip rock #1 before rock #2.** Whatever goes wrong with the first one will go wrong with the
second.

## Test checklist (each rock)
- [ ] Wakes on pick-up, yawns and sleeps after about a minute of stillness
- [ ] Purrs when petted, stops a moment after letting go
- [ ] Dizzy when shaken, giggles when it recovers
- [ ] Pedometer: 100 real steps count as roughly 80-120 in a pocket and in a hand; the cheer plays at
      the milestone; the count is still there after switching off and on
- [ ] Doesn't purr or wake by itself on a table or in a bag
- [ ] Volume comfortable at arm's length (not harsh, not shrill)
- [ ] Runs from the battery for at least several hours of normal use (measure it)
- [ ] Charges over USB-C with no unusual heat (check by touch after 15 minutes, supervised)
- [ ] 10 drops from about 1 m onto carpet: still works, shell closed, battery still in place
- [ ] 60 seconds of hard shaking: nothing rattles loose, no overheating
- [ ] Shell can't be opened without a screwdriver; no sharp edges or pinch points
- [ ] Switch is easy to find and works

## Fallbacks if time gets tight
- Drop the temperature color first (the LED shows only a fixed color).
- Then drop the haptic purr (sound and glow only).
- Last resort: wrap the working breadboard version in a printed shell. Don't ship anything that
  hasn't passed the battery and drop checks.
