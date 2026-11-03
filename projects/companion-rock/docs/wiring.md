# Wiring

Pin numbers live in `firmware/src/config.h`. The XIAO ESP32-S3 labels (D0-D10) map to GPIO numbers
as shown. **Check them against the Seeed pinout diagram before soldering.**

## Connections
| From | To | Notes |
|---|---|---|
| XIAO **3V3** | VIN/VCC of MPU-6050, AHT20, DRV2605L, MAX98357A, and the NeoPixel | One 3.3 V rail for everything. |
| XIAO **GND** | GND of everything | Common ground. |
| XIAO **D4** (GPIO5) | SDA on MPU-6050, AHT20, DRV2605L | I2C data. Boards usually have pull-ups already. |
| XIAO **D5** (GPIO6) | SCL on the same three boards | I2C clock. |
| XIAO **D0** (GPIO1) | Copper foil pad | Capacitive touch. Use a thin wire soldered to the foil. |
| XIAO **D1** (GPIO2) | NeoPixel **DIN**, through a 330 Ω resistor | Add a 100-470 uF capacitor across the LED's power if it flickers. |
| XIAO **D8** (GPIO7) | MAX98357A **BCLK** | I2S bit clock. |
| XIAO **D9** (GPIO8) | MAX98357A **LRC** | I2S word select. |
| XIAO **D10** (GPIO9) | MAX98357A **DIN** | I2S data. |
| MAX98357A **SD** | 3V3 (or leave per the board's default) | Check your breakout's notes for the SD/gain pins. |
| MAX98357A **+ / -** output | Speaker | |
| DRV2605L **motor +/-** | Vibration motor | Mount the motor firmly to the shell, not loose. |
| Battery **+** | Slide switch, then XIAO **BAT+** pad | Switch in line with the positive lead. |
| Battery **-** | XIAO **BAT-** pad | |

## Bench-test order (do each step before the next)
1. **XIAO alone** on USB-C: upload a blink sketch to confirm the toolchain works.
2. **Add the I2C boards one at a time.** Run an I2C scanner: expect 0x68, 0x38, 0x5A.
3. **Flash this firmware.** The serial monitor should print `sensors: mpu=1 aht=1 drv=1`.
4. **Touch pad.** Set `TOUCH_DEBUG 1` in `config.h`, watch the numbers while touching and not
   touching, then set `TOUCH_REL_THRESHOLD` so it triggers cleanly. The pad's foil should sit just
   behind the shell wall (about 1.2-1.6 mm of plastic) where hands go.
5. **Speaker.** Start at low volume (`AUDIO_VOLUME`). Check each of the five sounds.
6. **Haptic.** Pet it: the purr should be felt through the shell.
7. **On battery.** Check the power switch and charging (supervised).

## Gotchas to expect
- The ESP32-S3's touch readings rise on touch (unlike older ESP32s). The firmware accepts either
  direction, but the threshold still needs tuning.
- The AHT20 sits inside a warm shell, so it will read a few degrees high. The color only needs a
  rough trend, but keep the sensor away from the amplifier and the battery.
- The haptic driver's real-time mode is written for a coin (ERM) motor. If you use an LRA,
  set `HAPTIC_LRA 1` and run the driver library's auto-calibration example once.
- The motor, speaker and LED all draw from the 3.3 V regulator. If the rock resets when it purrs
  loudly, lower the volume or LED brightness first.
