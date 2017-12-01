#ifndef TURING_H
#define TURING_H

#include <stdint.h>

/* simple implementation of the turing machine
  by music thing modular. to be used as a mode
  later on

  shiftreg represents the two shift registers
  in the turing machine. the lower 8 bits forms
  a midi note number. see test_turing to see
  how it works. */

struct turing_t {
  uint16_t shiftreg;
  uint16_t next;
  int8_t idx_rand;
  int8_t idx_len;
  int8_t idx_range;
};

void turing_init(struct turing_t *t, uint16_t seed);
void turing_step_random(struct turing_t *t, int8_t rand);
void turing_step_length(struct turing_t *t, int8_t len);
void turing_step_range(struct turing_t *t, int8_t range);
uint8_t turing_get_length(struct turing_t *t);
uint8_t turing_clock(struct turing_t *t);

#endif /* TURING_H */
