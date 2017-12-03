#ifndef MODE_PRIO_H
#define MODE_PRIO_H

#include "mode.h"

typedef struct mode_prio_t {
  struct settings_t *settings;
  struct notemem_t *notemem;
  uint16_t *dac_values;
  struct output_t *out;
} mode_prio_t;

void mode_prio_event(mode_t *cxt, enum event ev);

#endif /* MODE_PRIO_H */
