#include "turing.h"
#include <stdio.h>

#ifdef DEBUG
#define LOG(...) { printf(__VA_ARGS__); }
#else
#define LOG(...)
#endif

uint16_t rand(struct turing_t *t)
{
  return (t->next = (t->next * 109 + 89) % 251);
}

void turing_init(struct turing_t *t, uint16_t seed)
{
  t->rand = 2;
  t->next = seed;
  t->shiftreg = 0;
  t->len = 16;
  t->range = 0;
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

void turing_set_range(struct turing_t *t, uint8_t range)
{
  LOG("turing_set_range: %u\n", range);
  if (range < 8)
    t->range = range;
}

uint8_t turing_clock(struct turing_t *t)
{
  uint16_t mask = 1 << (t->len - 1);
  uint16_t top = t->shiftreg & mask;

  if (t->rand < 11 && ((rand(t) & (t->rand - 1)) == 0)) {
    LOG("* flipped %u to %u! ", !!top, !top);
    top ^= mask;
  } else {
    LOG("                  ");
  }

  t->shiftreg = (top >> (t->len - 1)) | (t->shiftreg << 1);

  return (t->shiftreg & 0x00ff) >> t->range;
}

