# Companion Rock

A 3D-printed rock that reacts to being picked up, petted and shaken. Built as a
Christmas present: one for each kid.

**What it does**
- **Picked up:** wakes with a little chirp and a tiny buzz. The glow color follows the shell's
  temperature (cool blue, warm orange when it's held in warm hands).
- **Petted:** purrs. Soft vibration, a low humming sound and a slow pink glow.
- **Shaken:** gets dizzy. Rainbow swirl, wobbly sound, then a giggle when it recovers.
- **Left alone:** a yawn, then it falls asleep. Any pick-up wakes it.

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
- **Behavior logic (`firmware/lib/behavior`):** 34 checks pass on a normal computer. Run
  `cd firmware && make test`. The tests cover waking, sleeping, petting, shaking, rate-limiting
  sounds and the LED brightness cap.
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
