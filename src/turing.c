#include "turing.h"
#include <stdio.h>

#ifdef DEBUG
#define LOG(...) { printf(__VA_ARGS__); }
#else
#define LOG(...)
#endif

static uint16_t g_random[] = { 2, 4, 0xffff, 8, 16   };
static uint16_t g_length[] = { 2, 3, 4, 5, 8, 12, 16 };
static uint16_t g_range[]  = { 7, 6, 5, 4, 3         };

uint16_t rand(struct turing_t *t)
{
  return (t->next = (t->next * 109 + 89) % 251);
}

void turing_init(struct turing_t *t, uint16_t seed)
{
  t->next = seed;
  t->shiftreg = 0;
  t->idx_rand = 1;
  t->idx_len = 6;
  t->idx_range = 4;
}

void turing_step_random(struct turing_t *t, int8_t rand)
{
  uint8_t idx = t->idx_rand + rand;
  if (idx >= 0 && idx <= 6) {
    t->idx_rand = idx; 
    LOG("turing_step_random: %u\n", g_random[idx]);
  }
}

void turing_step_length(struct turing_t *t, int8_t len)
{
  uint8_t idx = t->idx_len + len;
  if (idx >= 0 && idx <= 6) {
    t->idx_len = idx;
    LOG("turing_step_length: %u\n", g_length[idx]);
  }
}

void turing_step_range(struct turing_t *t, int8_t range)
{
  uint8_t idx = t->idx_range + range;
  if (idx >= 0 && idx <= 4) {
    t->idx_range = idx;
    LOG("turing_step_range: %u\n", g_range[idx]);
  }
}

uint8_t turing_get_length(struct turing_t *t)
{
  return g_length[t->idx_len];
}

uint8_t turing_clock(struct turing_t *t)
{
  uint16_t mask = 1 << (g_length[t->idx_len] - 1);
  uint16_t top = t->shiftreg & mask;

  if (g_random[t->idx_rand] != 0xffff && ((rand(t) & (g_random[t->idx_rand] - 1)) == 0)) {
    LOG("* flipped %u to %u! ", !!top, !top);
    top ^= mask;
  } else {
    LOG("                  ");
  }

  t->shiftreg = (top >> (g_length[t->idx_len] - 1)) | (t->shiftreg << 1);

  return (t->shiftreg & 0x00ff) >> g_range[t->idx_range];
}

