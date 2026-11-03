// Companion Rock behavior core.
// Pure C++ with no Arduino dependencies, so it can be unit-tested on a normal
// computer (see ../test). The Arduino glue in ../../src/main.cpp feeds it sensor
// readings and plays back whatever it asks for.
//
// Design rules (on purpose, for a kids' toy):
//  - It reacts to being held, petted and shaken, then goes back to sleep.
//  - It never asks for attention, sulks, or "misses" anyone.
//  - Everything runs on the device: no microphone, no camera, no network.
#pragma once
#include <stdint.h>

namespace rock {

enum class State : uint8_t { Sleeping, Calm, Purring, Dizzy };

enum class Sound : uint8_t {
  None,
  WakeChirp,     // short rising chirp
  PurrStart,     // soft low hum that fades in
  Giggle,        // quick run of bright beeps
  DizzyWobble,   // warbling pitch
  SleepYawn      // slow falling tone
};

struct Inputs {
  uint32_t nowMs = 0;
  float accelG = 1.0f;     // total acceleration magnitude in g (1.0 = still)
  float gyroDps = 0.0f;    // total rotation rate in degrees per second
  bool touched = false;    // capacitive "pet" pad, already debounced by the glue
  float tempC = 22.0f;     // ambient/surface temperature near the shell
};

struct Outputs {
  State state = State::Calm;
  Sound sound = Sound::None;  // non-None only on the single update where it should start
  uint8_t hapticLevel = 0;    // 0..255 continuous vibration strength
  uint8_t r = 0, g = 0, b = 0; // LED color, brightness already applied
};

struct Config {
  uint32_t sleepAfterMs = 60000;      // stillness before it falls asleep
  float stillBandG = 0.08f;           // |accel - 1g| below this counts as still
  float stillGyroDps = 15.0f;
  float wakeBandG = 0.15f;            // movement needed to wake from sleep
  float wakeGyroDps = 25.0f;
  float shakeBandG = 0.7f;            // |accel - 1g| above this counts as a shake hit
  uint8_t shakeHitsForDizzy = 4;
  uint32_t shakeWindowMs = 800;       // hits must arrive this close together
  uint32_t shakeHitGapMs = 100;       // ignore hits closer together than this
  uint32_t dizzyMs = 3000;
  uint32_t touchReleaseMs = 400;      // stay purring briefly after letting go
  uint32_t soundMinGapMs = 800;       // never start sounds closer together than this
  uint8_t purrHaptic = 110;
  uint8_t wakeBuzzHaptic = 160;
  uint32_t wakeBuzzMs = 150;
  uint8_t maxBrightness = 140;        // LED cap (0..255) so it's never harsh
};

class Behavior {
 public:
  explicit Behavior(const Config& cfg = Config()) : cfg_(cfg) {}
  // Call every ~20-50 ms.
  Outputs update(const Inputs& in);
  State state() const { return state_; }

 private:
  void enter(State s, uint32_t now);
  bool soundAllowed(uint32_t now) const;
  uint8_t scale(uint8_t v) const;

  Config cfg_;
  State state_ = State::Calm;
  bool started_ = false;
  uint32_t stateSinceMs_ = 0;
  uint32_t lastActivityMs_ = 0;
  uint32_t lastTouchMs_ = 0;
  uint32_t lastSoundMs_ = 0;
  bool anySound_ = false;
  uint8_t shakeHits_ = 0;
  uint32_t lastShakeHitMs_ = 0;
  Sound pendingSound_ = Sound::None;
  bool wakeBuzz_ = false;
  uint32_t wakeBuzzSinceMs_ = 0;
};

}  // namespace rock
