# Progress

## Current state
- **Done:** concept and design rules, parts list, wiring plan, enclosure notes, build schedule,
  behavior logic and pedometer with host-side tests (all passing), draft hardware glue code.
- **Next:** order parts, then bench-test.
- **Known issues:** the hardware glue (`firmware/src`) has never been compiled against the real
  libraries or run on a board. See "Status of the code" in `README.md`.

## Log
### 2026.11.03 (later)
- Added the pedometer to the behavior core: rhythm-confirmed step detection, a lifetime count that
  is saved and restored, a gold flash and cheer every 500 steps, and serial `steps`/`reset`
  commands. 52 checks pass, and the pedometer checks fail when the detection is deliberately broken.
- Step detection was tuned on synthetic walking only; it needs real-world tuning on the rock.
### 2026.11.03
- Planned the project: two rocks, offline behavior only, no mic/camera/radio.
- Wrote and tested the behavior state machine (`make test`: all checks pass, and the tests fail when
  the logic is deliberately broken).
- Wrote the draft Arduino glue; it couldn't be built here (the sandbox couldn't download the ESP32
  toolchain), so it was only syntax-checked against stand-in headers.
