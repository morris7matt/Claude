// Host-side tests for the behavior core. Build and run with `make test`
// from the firmware folder. No hardware needed.
#include <stdio.h>
#include <stdlib.h>

#include "behavior.h"

using namespace rock;

static int failures = 0;
#define CHECK(cond, msg)                                              \
  do {                                                                \
    if (!(cond)) {                                                    \
      printf("FAIL  %s  (line %d)\n", msg, __LINE__);                 \
      failures++;                                                     \
    } else {                                                          \
      printf("ok    %s\n", msg);                                      \
    }                                                                 \
  } while (0)

// Helper that steps time at 20 ms and tracks the sounds it saw.
struct Sim {
  Behavior b;
  uint32_t now = 1000;
  Sound lastSound = Sound::None;
  int soundCount = 0;
  Outputs out;

  Outputs step(float accelG = 1.0f, float gyro = 0.0f, bool touched = false,
               float tempC = 22.0f) {
    Inputs in;
    in.nowMs = now;
    in.accelG = accelG;
    in.gyroDps = gyro;
    in.touched = touched;
    in.tempC = tempC;
    out = b.update(in);
    if (out.sound != Sound::None) {
      lastSound = out.sound;
      soundCount++;
    }
    now += 20;
    return out;
  }
  void run(uint32_t ms, float a = 1.0f, float g = 0.0f, bool t = false, float temp = 22.0f) {
    uint32_t end = now + ms;
    while (now < end) step(a, g, t, temp);
  }
};

static void test_boot_greeting() {
  Sim s;
  Outputs o = s.step();
  CHECK(o.state == State::Calm, "boots into Calm");
  CHECK(o.sound == Sound::WakeChirp, "boot plays wake chirp once");
  s.step();
  CHECK(s.soundCount == 1, "boot chirp is not repeated");
  CHECK(s.out.hapticLevel > 0, "wake buzz is felt on the next tick too");
  s.run(300);
  CHECK(s.out.hapticLevel == 0, "wake buzz ends after wakeBuzzMs");
}

static void test_falls_asleep_when_still() {
  Sim s;
  s.step();
  s.run(59000);
  CHECK(s.b.state() == State::Calm, "still awake at 59 s of stillness");
  s.run(2500);
  CHECK(s.b.state() == State::Sleeping, "asleep after 60 s of stillness");
  CHECK(s.lastSound == Sound::SleepYawn, "yawns when falling asleep");
  CHECK(s.out.r == 0 && s.out.g == 0 && s.out.b == 0, "LED is off while sleeping");
  CHECK(s.out.hapticLevel == 0, "no vibration while sleeping");
}

static void test_motion_keeps_awake() {
  Sim s;
  s.step();
  for (int i = 0; i < 100; i++) s.run(1000, 1.2f, 40.0f);  // carried around for 100 s
  CHECK(s.b.state() == State::Calm, "stays awake while being carried");
}

static void test_wakes_on_pickup() {
  Sim s;
  s.step();
  s.run(62000);
  CHECK(s.b.state() == State::Sleeping, "precondition: asleep");
  int before = s.soundCount;
  s.run(100, 1.0f, 5.0f);  // tiny vibration from a table must not wake it
  CHECK(s.b.state() == State::Sleeping, "tiny vibration does not wake it");
  s.run(100, 1.4f, 60.0f);
  CHECK(s.b.state() == State::Calm, "pick-up motion wakes it");
  CHECK(s.soundCount == before + 1 && s.lastSound == Sound::WakeChirp, "wake chirp on pickup");
}

static void test_petting() {
  Sim s;
  s.step();
  s.run(2000);
  Outputs o = s.step(1.0f, 0.0f, true);
  CHECK(o.state == State::Purring, "touch starts purring");
  CHECK(o.sound == Sound::PurrStart, "purr start sound");
  s.run(1000, 1.0f, 0.0f, true);
  CHECK(s.out.hapticLevel > 0, "purr vibration while touched");
  CHECK(s.out.r > 0, "purr glow while touched");
  s.run(200);  // brief release shorter than touchReleaseMs
  CHECK(s.b.state() == State::Purring, "keeps purring through a brief release");
  s.run(600);
  CHECK(s.b.state() == State::Calm, "back to Calm after letting go");
  CHECK(s.out.hapticLevel == 0, "vibration stops after letting go");
}

static void test_pet_from_sleep() {
  Sim s;
  s.step();
  s.run(62000);
  Outputs o = s.step(1.0f, 0.0f, true);
  CHECK(o.state == State::Purring, "petting a sleeping rock goes straight to purring");
}

static void test_shake_makes_dizzy_then_recovers() {
  Sim s;
  s.step();
  s.run(1000);
  // 4 hard hits ~150 ms apart
  for (int i = 0; i < 4; i++) {
    s.step(2.2f);
    s.run(140);
  }
  CHECK(s.b.state() == State::Dizzy, "four hard hits make it dizzy");
  CHECK(s.lastSound == Sound::DizzyWobble, "dizzy sound plays");
  CHECK(s.out.hapticLevel == 0, "no vibration while dizzy");
  s.run(3100);
  CHECK(s.b.state() == State::Calm, "recovers after dizzyMs");
  CHECK(s.lastSound == Sound::Giggle, "giggles when it recovers");
}

static void test_single_bump_is_not_a_shake() {
  Sim s;
  s.step();
  s.run(1000);
  s.step(2.5f);  // one drop-on-the-couch bump
  s.run(2000);
  CHECK(s.b.state() == State::Calm, "a single bump does not trigger dizzy");
}

static void test_slow_taps_are_not_a_shake() {
  Sim s;
  s.step();
  s.run(1000);
  for (int i = 0; i < 6; i++) {
    s.step(2.2f);
    s.run(1000);  // a second apart: outside the shake window
  }
  CHECK(s.b.state() != State::Dizzy, "hits a second apart are not a shake");
}

static void test_sound_rate_limit() {
  Sim s;
  s.step();
  s.run(1000);
  int before = s.soundCount;
  // rapid tapping: touch on/off every 100 ms for 2 s
  for (int i = 0; i < 20; i++) {
    s.run(100, 1.0f, 0.0f, i % 2 == 0);
  }
  CHECK(s.soundCount - before <= 3, "rapid tapping makes at most a few sounds");
}

static void test_brightness_cap() {
  Sim s;
  s.step();
  uint8_t maxSeen = 0;
  for (int i = 0; i < 400; i++) {
    Outputs o = s.step(1.0f, 0.0f, i % 100 < 50, 34.0f);
    if (o.r > maxSeen) maxSeen = o.r;
    if (o.g > maxSeen) maxSeen = o.g;
    if (o.b > maxSeen) maxSeen = o.b;
  }
  CHECK(maxSeen <= Config().maxBrightness, "LED never exceeds maxBrightness");
}

static void test_warm_color() {
  Sim cold, warm;
  cold.step(1.0f, 0.0f, false, 18.0f);
  warm.step(1.0f, 0.0f, false, 35.0f);
  Outputs c = cold.step(1.0f, 0.0f, false, 18.0f);
  Outputs w = warm.step(1.0f, 0.0f, false, 35.0f);
  CHECK(c.b > c.r, "cool shell glows blue");
  CHECK(w.r > w.b, "warm hands glow orange");
}

int main() {
  test_boot_greeting();
  test_falls_asleep_when_still();
  test_motion_keeps_awake();
  test_wakes_on_pickup();
  test_petting();
  test_pet_from_sleep();
  test_shake_makes_dizzy_then_recovers();
  test_single_bump_is_not_a_shake();
  test_slow_taps_are_not_a_shake();
  test_sound_rate_limit();
  test_brightness_cap();
  test_warm_color();
  printf("\n%s (%d failure%s)\n", failures ? "FAILED" : "ALL PASSED", failures,
         failures == 1 ? "" : "s");
  return failures ? 1 : 0;
}
