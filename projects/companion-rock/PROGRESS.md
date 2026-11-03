# Progress

## Current state
- **Done:** concept and design rules, parts list, wiring plan, enclosure notes, build schedule,
  behavior logic with host-side tests (all passing), draft hardware glue code.
- **Next:** order parts, then bench-test.
- **Known issues:** the hardware glue (`firmware/src`) has never been compiled against the real
  libraries or run on a board. See "Status of the code" in `README.md`.

## Log
### 2026.11.03
- Planned the project: two rocks, offline behavior only, no mic/camera/radio.
- Wrote and tested the behavior state machine (`make test`: all checks pass, and the tests fail when
  the logic is deliberately broken).
- Wrote the draft Arduino glue; it couldn't be built here (the sandbox couldn't download the ESP32
  toolchain), so it was only syntax-checked against stand-in headers.
