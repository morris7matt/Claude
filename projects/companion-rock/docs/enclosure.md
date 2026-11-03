# Enclosure notes

Goal: a rock-shaped shell in two halves that closes with screws, roughly 80-100 mm long. Design it
in Fusion 360 (or any CAD) around the real parts once they arrive. Measure them with calipers first.

## Approach
1. **Get a rock shape.** Download a free rock mesh (MakerWorld, Printables) or sculpt one in Fusion's
   Sculpt workspace. Scale it so the inside fits the parts below. Pick a shape with a flat-ish bottom
   so it sits still on a table.
2. **Hollow it** with a 2 mm wall (1.2-1.6 mm over the pet pad, see below).
3. **Split it in two** along a natural seam, ideally with a lip so the halves align.
4. **Add internal features:**
   - 4 screw bosses with M2 heat-set insert holes.
   - A closed battery pocket, padded with foam, away from the screws.
   - A speaker seat with a grille (a ring of small holes, 1.5-2 mm) on the side that faces up or out,
     with a small air gap behind it.
   - A mount for the vibration motor that clamps it to the shell wall.
   - Pockets or standoffs for the XIAO (USB-C port flush with a hole in the shell for charging),
     MPU-6050, AHT20 and DRV2605L boards.
   - A slot for the power switch you can reach from outside.
   - A small window for the LED, thin enough to glow through (0.6-1.0 mm of light PLA).
5. **Pet pad.** Stick copper tape on the inside wall where hands naturally rest, leaving 1.2-1.6 mm
   of plastic over it. Solder the wire to the foil before sticking it down.

## Space budget (approximate, check your own parts)
| Part | Roughly |
|---|---|
| XIAO ESP32-S3 | 21 x 17.8 mm |
| MPU-6050 (GY-521) | 21 x 16 mm |
| AHT20 board | 20 x 18 mm (varies) |
| DRV2605L board | 25 x 18 mm (varies) |
| MAX98357A board | 18 x 17 mm |
| Speaker | 28 mm diameter, 3-5 mm thick |
| ~1000 mAh LiPo | about 6 x 34 x 50 mm |

## Print settings (starting points)
- **Material:** PLA for the first prints. PETG is tougher for drops.
- **Walls:** 3 perimeters, 20-25% infill, 0.2 mm layers. Print the halves open side up.
- **Rock look:** multicolor speckle on the ACE, or a single gray with a sponge-dabbed paint finish.
- **First print:** run a cheap, low-detail fit-check shell with the real parts before the nice one.
- **Fasteners:** heat-set inserts in the bosses rather than screwing into plastic. Kids will open
  and close it, and plastic threads strip.

## Weight and feel
Add a bit of weight if it feels too light. It should feel like a real rock in the hand, not like a
toy. Pack foam so nothing rattles. Test by shaking it next to your ear.
