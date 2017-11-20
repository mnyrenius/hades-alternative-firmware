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
  uint8_t rand;
  uint16_t len;
};

void turing_init(struct turing_t *t);
void turing_set_random(struct turing_t *t, uint8_t rand);
void turing_set_length(struct turing_t *t, uint8_t len);
uint8_t turing_clock(struct turing_t *t);

#endif /* TURING_H */
