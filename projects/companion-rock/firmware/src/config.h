// Pin map and tunables for the XIAO ESP32-S3 build. See ../../docs/wiring.md.
#pragma once

// I2C bus shared by the motion sensor, temperature sensor and haptic driver.
#define PIN_SDA 5   // D4
#define PIN_SCL 6   // D5

// Capacitive "pet" pad. Must be a touch-capable pin (GPIO1-14 on the S3).
#define PIN_TOUCH 1   // D0

// Single NeoPixel (or a small ring) through a ~330 ohm series resistor.
#define PIN_NEOPIXEL 2   // D1
#define NEOPIXEL_COUNT 1

// MAX98357A I2S amplifier.
#define PIN_I2S_BCLK 7   // D8
#define PIN_I2S_LRC 8    // D9
#define PIN_I2S_DIN 9    // D10

// Speaker volume, 0.0 to 1.0. Start low: it is a toy for small ears.
#define AUDIO_VOLUME 0.30f

// Haptic motor type: 0 = coin/ERM (the usual one that ships with the breakout),
// 1 = LRA (nicer purr, but needs the driver's auto-calibration to be done).
#define HAPTIC_LRA 0

// Pet pad: touched when the raw reading moves more than this fraction away from
// its slowly-tracked untouched baseline. Raise it if the rock purrs by itself,
// lower it if it ignores you. Turn on TOUCH_DEBUG to watch the numbers.
#define TOUCH_REL_THRESHOLD 0.08f
#define TOUCH_DEBUG 0

// Loop rate for the sensors and behavior update.
#define UPDATE_INTERVAL_MS 25
