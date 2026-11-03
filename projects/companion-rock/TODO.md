# TODO

## Now
- [ ] Decide on the order: one rock each (assumed) or one shared. Adjust quantities in `docs/parts.md`.
- [ ] Verify the XIAO ESP32-S3 pin mapping (D0-D10 to GPIO) against Seeed's pinout before wiring.
- [ ] Order parts (target: delivered by Nov 16).
- [ ] Decide each kid's rock look (colors, speckle, name) and any personality tweaks to the sounds.

## When parts arrive
- [ ] Bench-test each board, then run the firmware (`pio run -t upload`).
- [ ] Fix whatever breaks on the first real build (library APIs, I2S, touch threshold).
- [ ] Tune touch threshold, volume, haptic strength, and the `Config` timings in `behavior.h`.
- [ ] Design and print the shell (`docs/enclosure.md`).

## Ideas, only if time allows
- [ ] Light sleep or deep sleep while asleep to stretch battery life (measure first).
- [ ] A hidden "quiet mode" (e.g. hold it upside down for 3 s to mute sounds), for car rides.
- [ ] Different sound sets per kid.
- [ ] Parent-only tuning mode over USB serial.
