#include "turing.h"
#include <stdio.h>

#ifdef DEBUG
#define LOG(...) { printf(__VA_ARGS__); }
#else
#define LOG(...)
#endif

static uint16_t randoms[] = { 2, 4, 0xffff, 8, 16   };
static uint16_t lengths[] = { 2, 3, 4, 5, 8, 12, 16 };
static uint16_t ranges[]  = { 7, 6, 5, 4, 3         };

uint16_t rand(turing_t *t)
{
  return (t->next = (t->next * 109 + 89) % 251);
}

void turing_init(turing_t *t, uint16_t seed)
{
  t->next = seed;
  t->shiftreg = 0;
  t->idx_rand = 1;
  t->idx_len = 6;
  t->idx_range = 4;
}

void turing_step_random(turing_t *t, int8_t rand)
{
  uint8_t idx = t->idx_rand + rand;
  if (idx >= 0 && idx <= 6) {
    t->idx_rand = idx; 
    LOG("turing_step_random: %u\n", randoms[idx]);
  }
}

void turing_step_length(turing_t *t, int8_t len)
{
  uint8_t idx = t->idx_len + len;
  if (idx >= 0 && idx <= 6) {
    t->idx_len = idx;
    LOG("turing_step_length: %u\n", lengths[idx]);
  }
}

void turing_step_range(turing_t *t, int8_t range)
{
  uint8_t idx = t->idx_range + range;
  if (idx >= 0 && idx <= 4) {
    t->idx_range = idx;
    LOG("turing_step_range: %u\n", ranges[idx]);
  }
}

uint8_t turing_get_length(turing_t *t)
{
  return lengths[t->idx_len];
}

uint8_t turing_clock(turing_t *t)
{
  uint16_t mask = 1 << (lengths[t->idx_len] - 1);
  uint16_t top = t->shiftreg & mask;

  if (randoms[t->idx_rand] != 0xffff && ((rand(t) & (randoms[t->idx_rand] - 1)) == 0)) {
    LOG("* flipped %u to %u! ", !!top, !top);
    top ^= mask;
  } else {
    LOG("                  ");
  }

  t->shiftreg = (top >> (lengths[t->idx_len] - 1)) | (t->shiftreg << 1);

  return (t->shiftreg & 0x00ff) >> ranges[t->idx_range];
}

