// Tiny non-blocking tone player for the MAX98357A over I2S.
// Uses the legacy ESP-IDF I2S driver (Arduino-ESP32 core 2.x, which is what the
// pinned platform in platformio.ini ships).
#pragma once
#include "behavior.h"

void audioBegin();
void audioPlay(rock::Sound s);
void audioPump();  // call every loop(); keeps the I2S buffer topped up
