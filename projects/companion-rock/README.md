# Companion Rock

A 3D-printed rock that reacts to being picked up, petted and shaken. Built as a
Christmas present: one for each kid.

**What it does**
- **Picked up:** wakes with a little chirp and a tiny buzz. The glow color follows the shell's
  temperature (cool blue, warm orange when it's held in warm hands).
- **Petted:** purrs. Soft vibration, a low humming sound and a slow pink glow.
- **Shaken:** gets dizzy. Rainbow swirl, wobbly sound, then a giggle when it recovers.
- **Left alone:** a yawn, then it falls asleep. Any pick-up wakes it.
- **Pedometer:** counts steps while it's carried (in a pocket, bag or hand). Every 500 steps it
  flashes gold and plays a happy little cheer. The count is saved, so it survives being switched
  off. It never nags: no goals, no "you haven't walked today", no guilt.

**Design rules (on purpose, it's a toy for kids)**
- It never asks for attention, sulks, or "misses" anyone. Put it down and it just sleeps.
- No microphone, no camera, no Wi-Fi or Bluetooth, no account, nothing leaves the rock.
- The behavior is a small state machine on the chip. No AI model is involved.
- Volume and LED brightness are capped low.

## Folder map
| Path | What's in it |
|---|---|
| `docs/parts.md` | Parts list, with what to check before you buy |
| `docs/wiring.md` | Pin-by-pin wiring and bench-test order |
| `docs/enclosure.md` | Shell design notes and print settings |
| `docs/build-plan.md` | Week-by-week schedule to Christmas, test and safety checklists |
| `firmware/` | PlatformIO project. `lib/behavior` is the logic, `src/` is the hardware glue |
| `PROGRESS.md`, `TODO.md` | Current state and open tasks |

## Status of the code
- **Behavior logic (`firmware/lib/behavior`):** 52 checks pass on a normal computer. Run
  `cd firmware && make test`. The tests cover waking, sleeping, petting, shaking, step counting,
  milestones, rate-limited sounds and the LED brightness cap.
- **Hardware glue (`firmware/src`):** **not compiled against the real libraries and never run on
  hardware.** It was written without a board and only syntax-checked against stand-in headers.
  Expect small fixes on first build: library API differences, touch thresholds, audio levels.
  Bench-test with one rock before building the second.

## Quick start (once parts arrive)
1. Install [PlatformIO](https://platformio.org/install) (VS Code extension or `pip install platformio`).
2. Wire the parts on a breadboard following `docs/wiring.md`.
3. `cd firmware && pio run -t upload && pio device monitor`
4. You should see `sensors: mpu=1 aht=1 drv=1`. A `0` means that sensor isn't wired or its I2C
   address isn't found.
5. Tune `src/config.h`: touch threshold, volume, haptic type.
6. Check the step counter (see below).

## Pedometer
- It looks for a steady rhythm of acceleration peaks (4 in a row, 0.25-1.5 s apart) before it
  counts, so one bump, a shake or random rattling doesn't count. It only counts while awake.
- The step count is saved to the chip's flash when the rock falls asleep and every 5 minutes while
  it's changing. Switching it off mid-walk can lose a few minutes of steps.
- Over USB, open the serial monitor and type `steps` to read the count or `reset` to zero it.
- **It's a toy, not a fitness tracker.** The thresholds in `behavior.h` (`stepThresholdG`,
  `stepsToConfirm`, ...) were tuned on simulated walking only. Walk 100 counted steps with the
  rock in a pocket, in a hand and in a bag, and adjust until the numbers are roughly right. Counts
  will differ by where it's carried, and that's fine for a toy.
