#ifndef MODE_MIDILEARN_H
#define MODE_MIDILEARN_H

#include "mode.h"

typedef struct mode_midilearn_t {
  struct settings_t *settings;
} mode_midilearn_t;

void mode_midilearn_event(mode_t *cxt, enum event ev);

#endif /* MODE_MIDILEARN_H */ 
