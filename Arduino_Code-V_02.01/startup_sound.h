#ifndef STARTUP_SOUND_H
#define STARTUP_SOUND_H

#include <Arduino.h>

extern void playDot();
extern void playDash();

void playCQ() {
  playDot();
  playDash();
  playDot();
  delay(200);
}

#endif
