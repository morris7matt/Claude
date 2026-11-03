// Companion Rock: reacts to being held, petted and shaken. No network, no mic.
#include <Adafruit_AHTX0.h>
#include <Adafruit_DRV2605.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Wire.h>

#include "audio.h"
#include "behavior.h"
#include "config.h"

namespace {

Adafruit_MPU6050 mpu;
Adafruit_AHTX0 aht;
Adafruit_DRV2605 drv;
Adafruit_NeoPixel pixel(NEOPIXEL_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
rock::Behavior behavior;

bool haveMpu = false, haveAht = false, haveDrv = false;
float tempC = 22.0f;
uint32_t lastAhtMs = 0;
uint32_t lastUpdateMs = 0;
uint8_t lastHaptic = 255;

// Touch pad state: baseline tracks slow drift; 3 consecutive hits = touched.
float touchBaseline = 0;
uint8_t touchHits = 0, touchMisses = 0;
bool touched = false;

void calibrateTouch() {
  // Leave the rock untouched for the first second after power-on.
  double sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += touchRead(PIN_TOUCH);
    delay(10);
  }
  touchBaseline = (float)(sum / 30.0);
}

void updateTouch() {
  const float raw = (float)touchRead(PIN_TOUCH);
  const float delta = raw > touchBaseline ? raw - touchBaseline : touchBaseline - raw;
  const bool over = delta > touchBaseline * TOUCH_REL_THRESHOLD;
  if (over) {
    touchHits = touchHits < 255 ? touchHits + 1 : 255;
    touchMisses = 0;
  } else {
    touchMisses = touchMisses < 255 ? touchMisses + 1 : 255;
    touchHits = 0;
    touchBaseline += (raw - touchBaseline) * 0.01f;  // follow slow drift while untouched
  }
  if (touchHits >= 3) touched = true;
  if (touchMisses >= 3) touched = false;
#if TOUCH_DEBUG
  Serial.printf("touch raw=%.0f base=%.0f delta=%.0f touched=%d\n", raw, touchBaseline, delta,
                touched);
#endif
}

void setHaptic(uint8_t level) {
  if (!haveDrv || level == lastHaptic) return;
  lastHaptic = level;
  // Real-time playback takes a signed 8-bit value by default: 0..127 is forward drive.
  drv.setRealtimeValue(level > 254 ? 127 : level / 2);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);

  pixel.begin();
  pixel.clear();
  pixel.show();

  haveMpu = mpu.begin();
  if (haveMpu) {
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  haveAht = aht.begin();
  haveDrv = drv.begin();
  if (haveDrv) {
#if HAPTIC_LRA
    drv.useLRA();
#else
    drv.useERM();
#endif
    drv.setMode(DRV2605_MODE_REALTIME);
    drv.setRealtimeValue(0);
  }
  Serial.printf("sensors: mpu=%d aht=%d drv=%d\n", haveMpu, haveAht, haveDrv);

  audioBegin();
  calibrateTouch();
  lastUpdateMs = millis();
}

void loop() {
  audioPump();  // cheap; keeps sound smooth between sensor updates

  const uint32_t now = millis();
  if (now - lastUpdateMs < UPDATE_INTERVAL_MS) return;
  lastUpdateMs = now;

  rock::Inputs in;
  in.nowMs = now;
  in.tempC = tempC;

  if (haveMpu) {
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    const float ax = a.acceleration.x, ay = a.acceleration.y, az = a.acceleration.z;
    in.accelG = sqrtf(ax * ax + ay * ay + az * az) / 9.80665f;
    const float gx = g.gyro.x, gy = g.gyro.y, gz = g.gyro.z;
    in.gyroDps = sqrtf(gx * gx + gy * gy + gz * gz) * 57.29578f;
  }

  if (haveAht && now - lastAhtMs > 1000) {
    sensors_event_t hum, temp;
    aht.getEvent(&hum, &temp);
    tempC = temp.temperature;
    in.tempC = tempC;
    lastAhtMs = now;
  }

  updateTouch();
  in.touched = touched;

  const rock::Outputs out = behavior.update(in);

  if (out.sound != rock::Sound::None) audioPlay(out.sound);
  setHaptic(out.hapticLevel);
  pixel.setPixelColor(0, pixel.Color(out.r, out.g, out.b));
  pixel.show();
}
