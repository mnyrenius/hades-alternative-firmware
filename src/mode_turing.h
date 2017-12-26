#ifndef MODE_TURING_H
#define MODE_TURING_H

#include "mode.h"

typedef struct mode_turing_t {
  struct settings_t *settings;
  struct turing_t *turing;
  uint16_t *dac_values;
  struct output_t *out;
  uint8_t running;
  uint8_t clk_count;
} mode_turing_t;

void mode_turing_event(mode_t *cxt, enum event ev);

#endif /* MODE_TURING_H */ 
