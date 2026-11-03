# Companion Rock

3D-printed, offline, kid-safe sensor toy. See README.md.

## Rules
- Keep the toy a toy: no microphone, camera, radio or cloud; it never begs for attention.
- Behavior logic lives in `firmware/lib/behavior` (pure C++). Change it only with tests: run
  `cd firmware && make test` and add a test for any new behavior.
- Hardware glue in `firmware/src` is uncompiled against real libraries until a board exists. Say so
  when reporting status.
- Never recommend charging unattended or shipping a rock that hasn't passed `docs/build-plan.md`
  test checklist.
