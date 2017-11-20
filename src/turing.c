#include "turing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef DEBUG
#define LOG(...) { printf(__VA_ARGS__); }
#else
#define LOG(...)
#endif

void turing_init(struct turing_t *t)
{
  t->rand = 2;
  t->shiftreg = 0;
  t->len = 16;
  srand(time(0));
}

void turing_set_random(struct turing_t *t, uint8_t rand)
{
  LOG("turing_set_random: %u\n", rand);
  if (rand < 12)
    t->rand = rand;
}

void turing_set_length(struct turing_t *t, uint8_t len)
{
  LOG("turing_set_length: %u\n", len);
  if (len <= 16)
    t->len = len;
}

uint8_t turing_clock(struct turing_t *t)
{
  uint16_t mask = 1 << (t->len - 1);
  uint16_t top = t->shiftreg & mask;

  if (t->rand < 11 && rand() % t->rand == 0) {
    LOG("* flipped %u to %u! ", top == 1, !top);
    top ^= mask;
  } else {
    LOG("                  ");
  }

  t->shiftreg = (top >> (t->len - 1)) | (t->shiftreg << 1);

  return t->shiftreg;
}

