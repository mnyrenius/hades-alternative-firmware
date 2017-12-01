#ifndef MODE_PRIO_H
#define MODE_PRIO_H

#include "mode.h"

struct settings_t;
struct notemem_t;
struct output_state_t;

struct mode_prio_t {
  struct mode_t base;
  struct settings_t *settings;
  struct notemem_t *notemem;
  uint16_t *dac_values;
  struct output_state_t *state;
};

void mode_prio_init(struct mode_t *cxt);
void mode_prio_note_on(struct mode_t *cxt, uint8_t channel, uint8_t note);
void mode_prio_note_off(struct mode_t *cxt, uint8_t channel, uint8_t note);

#endif /* MODE_PRIO_H */
