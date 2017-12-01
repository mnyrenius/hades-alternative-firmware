#ifndef MODE_TURING_H
#define MODE_TURING_H

#include "mode.h"

struct settings_t;
struct turing_t;
struct output_state_t;

struct mode_turing_t {
  struct mode_t base;
  struct settings_t *settings;
  struct turing_t *turing;
  uint16_t *dac_values;
  struct output_state_t *state;
};

void mode_turing_init(struct mode_turing_t *cxt);
void mode_turing_exit(struct mode_turing_t *cxt);
void mode_turing_note_on(struct mode_turing_t *cxt, uint8_t channel, uint8_t note);
void mode_turing_note_off(struct mode_turing_t *cxt, uint8_t channel, uint8_t note);
void mode_turing_clock(struct mode_turing_t *cxt);
void mode_turing_update(struct mode_turing_t *cxt);

#endif /* MODE_TURING_H */ 
