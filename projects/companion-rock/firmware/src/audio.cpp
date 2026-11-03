#include "audio.h"

#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

#include "config.h"

namespace {

constexpr i2s_port_t kPort = I2S_NUM_0;
constexpr uint32_t kSampleRate = 16000;
constexpr size_t kBlock = 128;

// One piece of a sound: a tone gliding from hz0 to hz1 (hz0 == 0 means silence).
struct Segment {
  float hz0, hz1;
  uint16_t ms;
  float vibHz, vibDepthHz;  // pitch wobble
  float amHz;               // amplitude "purr" modulation (0 = off)
};

const Segment kWake[] = {{500, 900, 110, 0, 0, 0}, {900, 1300, 110, 0, 0, 0}};
const Segment kPurr[] = {{160, 185, 800, 0, 0, 24}};
const Segment kGiggle[] = {
    {800, 1000, 70, 0, 0, 0},  {0, 0, 45, 0, 0, 0}, {900, 1100, 70, 0, 0, 0},
    {0, 0, 45, 0, 0, 0},       {1000, 1250, 70, 0, 0, 0}, {0, 0, 45, 0, 0, 0},
    {1100, 1400, 90, 0, 0, 0}};
const Segment kDizzy[] = {{650, 380, 1300, 6, 110, 0}};
const Segment kYawn[] = {{720, 300, 1000, 0, 0, 0}};
// C5, E5, G5, C6: a happy little arpeggio.
const Segment kCheer[] = {{523, 523, 90, 0, 0, 0},  {659, 659, 90, 0, 0, 0},
                          {784, 784, 90, 0, 0, 0},  {1047, 1047, 220, 0, 0, 0}};

const Segment* gSeq = nullptr;
size_t gSeqLen = 0;
size_t gSeg = 0;
uint32_t gSegSample = 0;  // samples played in the current segment
float gPhase = 0;
float gVibPhase = 0;
float gAmPhase = 0;

int16_t gPend[kBlock];
size_t gPendLen = 0;
size_t gPendPos = 0;

uint32_t segSamples(const Segment& s) { return (uint32_t)s.ms * kSampleRate / 1000; }

// Generates up to kBlock samples into gPend. Returns false when nothing is playing.
bool fillBlock() {
  if (gSeq == nullptr) return false;
  for (size_t i = 0; i < kBlock; i++) {
    if (gSeg >= gSeqLen) {
      gSeq = nullptr;
      gPendLen = i;
      gPendPos = 0;
      for (size_t k = i; k < kBlock; k++) gPend[k] = 0;
      return i > 0;
    }
    const Segment& s = gSeq[gSeg];
    const uint32_t total = segSamples(s);
    float sample = 0;
    if (s.hz0 > 0) {
      const float t = (float)gSegSample / (float)total;
      float hz = s.hz0 + (s.hz1 - s.hz0) * t;
      if (s.vibHz > 0) {
        hz += s.vibDepthHz * sinf(gVibPhase);
        gVibPhase += 2.0f * (float)M_PI * s.vibHz / kSampleRate;
      }
      gPhase += 2.0f * (float)M_PI * hz / kSampleRate;
      if (gPhase > 2.0f * (float)M_PI) gPhase -= 2.0f * (float)M_PI;
      float amp = AUDIO_VOLUME;
      // Short fade in/out so segments never click.
      const uint32_t fade = kSampleRate / 125;  // 8 ms
      if (gSegSample < fade) amp *= (float)gSegSample / fade;
      if (total - gSegSample < fade) amp *= (float)(total - gSegSample) / fade;
      if (s.amHz > 0) {
        amp *= 0.65f + 0.35f * sinf(gAmPhase);
        gAmPhase += 2.0f * (float)M_PI * s.amHz / kSampleRate;
      }
      sample = sinf(gPhase) * amp;
    }
    gPend[i] = (int16_t)(sample * 30000.0f);
    if (++gSegSample >= total) {
      gSeg++;
      gSegSample = 0;
    }
  }
  gPendLen = kBlock;
  gPendPos = 0;
  return true;
}

}  // namespace

void audioBegin() {
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  cfg.sample_rate = kSampleRate;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  cfg.dma_buf_count = 6;
  cfg.dma_buf_len = 256;
  cfg.tx_desc_auto_clear = true;  // plays silence when we write nothing

  i2s_pin_config_t pins = {};
  pins.mck_io_num = I2S_PIN_NO_CHANGE;
  pins.bck_io_num = PIN_I2S_BCLK;
  pins.ws_io_num = PIN_I2S_LRC;
  pins.data_out_num = PIN_I2S_DIN;
  pins.data_in_num = I2S_PIN_NO_CHANGE;

  i2s_driver_install(kPort, &cfg, 0, nullptr);
  i2s_set_pin(kPort, &pins);
}

void audioPlay(rock::Sound s) {
  using rock::Sound;
  switch (s) {
    case Sound::WakeChirp:   gSeq = kWake;   gSeqLen = sizeof(kWake) / sizeof(Segment); break;
    case Sound::PurrStart:   gSeq = kPurr;   gSeqLen = sizeof(kPurr) / sizeof(Segment); break;
    case Sound::Giggle:      gSeq = kGiggle; gSeqLen = sizeof(kGiggle) / sizeof(Segment); break;
    case Sound::DizzyWobble: gSeq = kDizzy;  gSeqLen = sizeof(kDizzy) / sizeof(Segment); break;
    case Sound::SleepYawn:   gSeq = kYawn;   gSeqLen = sizeof(kYawn) / sizeof(Segment); break;
    case Sound::StepCheer:   gSeq = kCheer;  gSeqLen = sizeof(kCheer) / sizeof(Segment); break;
    case Sound::None:        return;
  }
  gSeg = 0;
  gSegSample = 0;
  gPhase = gVibPhase = gAmPhase = 0;
  gPendLen = gPendPos = 0;
}

void audioPump() {
  for (int guard = 0; guard < 8; guard++) {
    if (gPendPos >= gPendLen && !fillBlock()) return;
    size_t written = 0;
    i2s_write(kPort, &gPend[gPendPos], (gPendLen - gPendPos) * sizeof(int16_t), &written, 0);
    gPendPos += written / sizeof(int16_t);
    if (gPendPos < gPendLen) return;  // DMA buffer is full; try again next loop
  }
}
