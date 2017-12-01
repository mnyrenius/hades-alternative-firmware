#ifndef MODE_MIDILEARN_H
#define MODE_MIDILEARN_H

#include "mode.h"

struct settings_t;

struct mode_midilearn_t {
  struct mode_t base;
  struct settings_t *settings;
};

void mode_midilearn_init(struct mode_t *cxt);
void mode_midilearn_note_on(struct mode_t *cxt, uint8_t channel, uint8_t note);
void mode_midilearn_note_off(struct mode_t *cxt, uint8_t channel, uint8_t note);

#endif /* MODE_MIDILEARN_H */ 
