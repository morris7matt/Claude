# Parts list

Quantities are for **two rocks plus one spare set** of the small, easy-to-break boards. I haven't
checked prices or stock; look them up before ordering. Buy from known shops (Adafruit, SparkFun,
Seeed Studio, DigiKey, or Amazon items sold *and shipped* by the brand). Avoid long AliExpress
shipping: it's early November, and everything should be in your hands by mid-November.

| # | Part | Qty | Notes |
|---|---|---|---|
| 1 | **Seeed Studio XIAO ESP32-S3** (the plain one) | 3 | **Not** the "Sense" version, which adds a camera/microphone. It has built-in LiPo charging over USB-C. |
| 2 | **MPU-6050 breakout** (GY-521 or similar) | 3 | Accelerometer + gyroscope. Alternative: an LSM6DS3 board, but the code is written for the MPU-6050. |
| 3 | **AHT20 breakout** (I2C temperature/humidity) | 3 | Alternative: SHT31. Any I2C board works if the library changes. |
| 4 | **DRV2605L haptic motor driver breakout** | 3 | Adafruit's board is the usual one. |
| 5 | **Coin/ERM vibration motor, ~10 mm** | 3 | The Adafruit DRV2605L kit usually includes one. LRA motors feel nicer but need calibration. |
| 6 | **MAX98357A I2S amplifier breakout** | 3 | Mono, 3 W class D. |
| 7 | **Thin speaker, 4 Ω, about 28 mm, 2-3 W** | 3 | Small and flat so it fits in the shell. Keep it under 30 mm. |
| 8 | **NeoPixel / WS2812B LED**, diffused | 3 | One single LED per rock, or a 7-LED "Jewel" for a nicer glow (set `NEOPIXEL_COUNT` in `config.h`). |
| 9 | **Single-cell 3.7 V LiPo, about 500-1000 mAh, with a built-in protection circuit** | 2 (+1 spare) | Roughly 6 x 34 x 50 mm for ~1000 mAh. **See battery safety below.** |
| 10 | **Mini slide switch (SPDT)** | 3 | Inline with the battery positive lead, as the power switch. |
| 11 | **Copper foil tape (conductive adhesive)** | 1 roll | The pet pad. |
| 12 | **330 Ω resistor** | 3 | In series with the NeoPixel data line. |
| 13 | **Silicone-insulated hookup wire, ~30 AWG** | 1 set | Several colors. |
| 14 | **M2 screws + M2 heat-set inserts, assorted** | 1 kit | For closing the shell. A soldering iron tip made for inserts helps. |
| 15 | **Breadboard, jumper wires, USB-C cable** | 1 | For the bench-test stage. |
| 16 | **Kapton tape, thin foam or felt, hot glue** | 1 | Padding, insulation, strain relief. |
| 17 | **Filament** | 1+ | PLA or PETG; your ACE multicolor setup is great for a speckled rock. |

Soldering iron, solder, flush cutters and a multimeter are assumed.

## Before you buy
- **Battery connector polarity.** Many LiPo cells ship with a JST plug wired backwards from the board
  you're using. The XIAO's battery connection is two solder pads on the back, so you won't be using
  a plug; solder the leads to the pads marked BAT+ and BAT- (check the Seeed wiki).
  Verify polarity with a multimeter before connecting anything.
- **Protected cells only.** Buy a cell that says it has a protection circuit (PCM/PCB). Do not use a
  bare cell.
- **I2C addresses.** MPU-6050 is 0x68, AHT20 is 0x38, DRV2605L is 0x5A. All different, so all three
  can share two wires.
- **Check the speaker size** against the shell before ordering. 28 mm is about the limit.

## Battery safety (it's going to a child)
- Charge only through the XIAO's USB-C port, with the rock on a hard surface, supervised. Don't
  leave it charging overnight.
- Close the shell with screws into heat-set inserts. No snap-fit lid, so a kid can't open it.
- Pad the cell so it can't get pinched, punctured or squeezed by a lid or screw.
- If it ever swells, gets hot or smells, stop using it and put it somewhere non-flammable.
- Do a drop test before gifting (see `build-plan.md`).
