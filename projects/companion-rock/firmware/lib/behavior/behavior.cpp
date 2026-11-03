#include "behavior.h"

#include <math.h>

namespace rock {

namespace {

float absf(float v) { return v < 0 ? -v : v; }

// h in 0..255 -> fully saturated color, v is brightness 0..255.
void hueToRgb(uint8_t h, uint8_t v, uint8_t& r, uint8_t& g, uint8_t& b) {
  uint8_t region = h / 43;
  uint8_t rem = (h - region * 43) * 6;  // 0..255 within the region
  uint8_t q = (uint16_t(v) * (255 - rem)) / 255;
  uint8_t t = (uint16_t(v) * rem) / 255;
  switch (region) {
    case 0: r = v; g = t; b = 0; break;
    case 1: r = q; g = v; b = 0; break;
    case 2: r = 0; g = v; b = t; break;
    case 3: r = 0; g = q; b = v; break;
    case 4: r = t; g = 0; b = v; break;
    default: r = v; g = 0; b = q; break;
  }
}

// Triangle wave 0..255 with the given period.
uint8_t triangle(uint32_t nowMs, uint32_t periodMs) {
  uint32_t p = nowMs % periodMs;
  uint32_t half = periodMs / 2;
  uint32_t x = p < half ? p : periodMs - p;
  return (uint8_t)((x * 255) / half);
}

uint8_t lerp8(uint8_t a, uint8_t b, float t) {
  return (uint8_t)(a + (b - a) * t);
}

}  // namespace

uint8_t Behavior::scale(uint8_t v) const {
  return (uint8_t)((uint16_t(v) * cfg_.maxBrightness) / 255);
}

bool Behavior::soundAllowed(uint32_t now) const {
  return !anySound_ || (now - lastSoundMs_) >= cfg_.soundMinGapMs;
}

void Behavior::enter(State s, uint32_t now) {
  state_ = s;
  stateSinceMs_ = now;
  switch (s) {
    case State::Sleeping: pendingSound_ = Sound::SleepYawn; break;
    case State::Purring:  pendingSound_ = Sound::PurrStart; break;
    case State::Dizzy:    pendingSound_ = Sound::DizzyWobble; break;
    case State::Calm:     break;  // the caller picks the sound (wake vs. back from dizzy)
  }
}

void Behavior::updateSteps(const Inputs& in) {
  const uint32_t now = in.nowMs;
  if (!stepInit_) {
    stepInit_ = true;
    accelLp_ = accelBase_ = in.accelG;
    dev1_ = dev2_ = 0;
    t1_ = now;
    return;
  }

  accelLp_ += 0.4f * (in.accelG - accelLp_);    // smooth out sensor noise
  accelBase_ += 0.02f * (accelLp_ - accelBase_);  // slow "gravity" baseline
  const float dev = accelLp_ - accelBase_;

  // The sample before this one was a peak if it was higher than both neighbours.
  const bool counting = state_ == State::Calm || state_ == State::Purring;
  if (dev1_ > dev2_ && dev1_ >= dev && dev1_ > cfg_.stepThresholdG) {
    if (!counting) {
      streak_ = 0;
      hasPeak_ = false;
    } else if (!hasPeak_ || (t1_ - lastPeakMs_) > cfg_.stepMaxGapMs) {
      streak_ = 1;  // start of a possible walk
      hasPeak_ = true;
      lastPeakMs_ = t1_;
    } else if ((t1_ - lastPeakMs_) >= cfg_.stepMinGapMs) {
      streak_ = streak_ < 255 ? streak_ + 1 : 255;
      lastPeakMs_ = t1_;
      const uint32_t before = steps_;
      if (streak_ == cfg_.stepsToConfirm) {
        steps_ += cfg_.stepsToConfirm;  // the confirming peaks count retroactively
      } else if (streak_ > cfg_.stepsToConfirm) {
        steps_ += 1;
      }
      if (cfg_.milestoneSteps > 0 && steps_ / cfg_.milestoneSteps > before / cfg_.milestoneSteps) {
        celebrateUntilMs_ = now + cfg_.celebrateMs;
        if (cfg_.milestoneSound) pendingSound_ = Sound::StepCheer;
      }
    }
    // else: a second bump inside the same step; ignore it.
  }
  if (hasPeak_ && (now - lastPeakMs_) > cfg_.stepMaxGapMs) {
    streak_ = 0;
    hasPeak_ = false;
  }
  if (!counting) {
    streak_ = 0;
    hasPeak_ = false;
  }

  dev2_ = dev1_;
  dev1_ = dev;
  t1_ = now;
}

Outputs Behavior::update(const Inputs& in) {
  const uint32_t now = in.nowMs;
  pendingSound_ = Sound::None;

  if (!started_) {
    started_ = true;
    lastActivityMs_ = now;
    lastTouchMs_ = 0;
    enter(State::Calm, now);
    pendingSound_ = Sound::WakeChirp;  // boot greeting
  }

  const float accelDev = absf(in.accelG - 1.0f);
  const bool moving = accelDev > cfg_.stillBandG || in.gyroDps > cfg_.stillGyroDps;
  if (moving || in.touched) lastActivityMs_ = now;
  if (in.touched) lastTouchMs_ = now;

  // Shake detection: several big hits in quick succession.
  if (accelDev > cfg_.shakeBandG && (now - lastShakeHitMs_) >= cfg_.shakeHitGapMs) {
    if (shakeHits_ > 0 && (now - lastShakeHitMs_) > cfg_.shakeWindowMs) shakeHits_ = 0;
    shakeHits_++;
    lastShakeHitMs_ = now;
  } else if (shakeHits_ > 0 && (now - lastShakeHitMs_) > cfg_.shakeWindowMs) {
    shakeHits_ = 0;
  }
  const bool shaken = shakeHits_ >= cfg_.shakeHitsForDizzy;

  const bool touchActive = in.touched || (now - lastTouchMs_) < cfg_.touchReleaseMs;

  switch (state_) {
    case State::Sleeping: {
      const bool wakeMove = accelDev > cfg_.wakeBandG || in.gyroDps > cfg_.wakeGyroDps;
      if (in.touched) {
        enter(State::Purring, now);
      } else if (wakeMove) {
        enter(State::Calm, now);
        pendingSound_ = Sound::WakeChirp;
      }
      break;
    }
    case State::Calm: {
      if (shaken) {
        shakeHits_ = 0;
        enter(State::Dizzy, now);
      } else if (in.touched) {
        enter(State::Purring, now);
      } else if ((now - lastActivityMs_) >= cfg_.sleepAfterMs) {
        enter(State::Sleeping, now);
      }
      break;
    }
    case State::Purring: {
      if (shaken) {
        shakeHits_ = 0;
        enter(State::Dizzy, now);
      } else if (!touchActive) {
        enter(State::Calm, now);
      }
      break;
    }
    case State::Dizzy: {
      if ((now - stateSinceMs_) >= cfg_.dizzyMs) {
        enter(State::Calm, now);
        pendingSound_ = Sound::Giggle;  // recovers with a giggle
      }
      break;
    }
  }

  updateSteps(in);

  if (pendingSound_ == Sound::WakeChirp) {
    wakeBuzz_ = true;
    wakeBuzzSinceMs_ = now;
  }

  // Rate-limit sounds so rapid taps or shakes can't turn into noise.
  Sound sound = pendingSound_;
  if (sound != Sound::None) {
    if (soundAllowed(now)) {
      lastSoundMs_ = now;
      anySound_ = true;
    } else {
      sound = Sound::None;
    }
  }

  Outputs out;
  out.state = state_;
  out.sound = sound;
  out.steps = steps_;

  switch (state_) {
    case State::Sleeping:
      out.r = out.g = out.b = 0;
      out.hapticLevel = 0;
      break;
    case State::Calm: {
      // Cool blue when the shell is cool, warm orange when held in warm hands.
      float t = (in.tempC - 20.0f) / 14.0f;
      t = t < 0 ? 0 : (t > 1 ? 1 : t);
      out.r = scale(lerp8(40, 255, t));
      out.g = scale(lerp8(90, 120, t));
      out.b = scale(lerp8(255, 20, t));
      out.hapticLevel = (wakeBuzz_ && (now - wakeBuzzSinceMs_) < cfg_.wakeBuzzMs)
                            ? cfg_.wakeBuzzHaptic : 0;
      break;
    }
    case State::Purring: {
      const uint8_t breath = triangle(now, 1600);  // slow pulse
      const uint8_t v = scale((uint8_t)(90 + (breath * 165) / 255));
      out.r = v;
      out.g = (uint8_t)((uint16_t(v) * 55) / 255);
      out.b = (uint8_t)((uint16_t(v) * 120) / 255);
      out.hapticLevel = cfg_.purrHaptic;
      break;
    }
    case State::Dizzy: {
      hueToRgb((uint8_t)((now / 6) & 0xFF), scale(255), out.r, out.g, out.b);
      out.hapticLevel = 0;  // no buzzing while shaken
      break;
    }
  }

  // Milestone sparkle: blinking gold over whatever else is showing.
  if (now < celebrateUntilMs_ && state_ != State::Sleeping && state_ != State::Dizzy) {
    const uint8_t blink = triangle(now, 400);
    const uint8_t v = scale((uint8_t)(80 + (blink * 175) / 255));
    out.r = v;
    out.g = (uint8_t)((uint16_t(v) * 170) / 255);
    out.b = 0;
  }
  return out;
}

}  // namespace rock
