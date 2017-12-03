#include <stdio.h>
#include <assert.h>
#include "ringbuffer.h"
#include "notemem.h"
#include "turing.h"
#include <time.h>

#define RB_WRITE(rb, value) {\
  ringbuffer_write(rb, value); \
}

#define RB_READ(rb, expected) {\
  { \
    uint8_t value; \
    assert(ringbuffer_read(rb, &value) == 0); \
    assert(value == expected); \
  } \
}

#define RB_EXPECT_EMPTY(rb) {\
  assert(ringbuffer_read(rb, 0) != 0); \
}

void test_ringbuffer(void)
{
  ringbuffer_t rb;
  ringbuffer_init(&rb);

  for (int i = 1; i < 32; ++i) {
    RB_WRITE(&rb, i);
  }

  for (int i = 1; i < 32; ++i) {
    RB_READ(&rb, i);
  }
  RB_EXPECT_EMPTY(&rb);

  RB_WRITE(&rb, 1);
  RB_WRITE(&rb, 3);
  RB_READ(&rb, 1);
  RB_WRITE(&rb, 5);
  RB_READ(&rb, 3);
  RB_READ(&rb, 5);

  RB_EXPECT_EMPTY(&rb);

  printf("TEST RINGBUFFER OK!\n");
}

void notemem_print(notemem_t *nm)
{
  printf("(%u, [", nm->active);
  for (node_t *n = nm->top; n; n = n->next) {
    printf("%u, ", n->note);
  }
  printf("])\n");
}

#define NOTE_ON(nm, note) {\
  notemem_note_on(nm, note); \
  notemem_print(nm); \
}

#define NOTE_OFF(nm, note, expected) {\
  assert(notemem_note_off(nm, note) == expected); \
  notemem_print(nm); \
}

void test_notemem(void)
{
  notemem_t nm;
  notemem_init(&nm, NM_PRIO_HIGH);

  uint8_t note_ons[10]  = { 1, 6, 5, 8, 4, 2, 7, 10, 3, 12 };
  uint8_t note_offs[10] = { 3, 10, 12, 7, 2, 8, 6, 1, 4, 5 };

  uint8_t expected[NM_PRIO_ENUM_END][10] = {
    { 12, 12, 8, 8, 8, 6, 5, 5, 5, 0xff }, // high
    { 1, 1, 1, 1, 1, 1, 1, 4, 5  , 0xff }, // low
    { 12, 12, 7, 2, 4, 4, 4, 4, 5, 0xff }, // last
  };

  for (int prio = NM_PRIO_HIGH; prio < NM_PRIO_ENUM_END; ++prio) {
    notemem_init(&nm, prio);
    printf("Pushing...\n");
    for (int i = 0; i < sizeof(note_ons); ++i) {
      NOTE_ON(&nm, note_ons[i]);
    }

    printf("Popping...\n");
    for (int i = 0; i < sizeof(note_ons); ++i) {
      NOTE_OFF(&nm, note_offs[i], expected[prio][i]);
    }
  }

  NOTE_ON(&nm, 1);
  for (int i = 0; i < 32; ++i) {
    NOTE_ON(&nm, 2);
    NOTE_OFF(&nm, 2, 1);
  }
  NOTE_OFF(&nm, 1, 0xff);

  printf("TEST NOTEMEM OK!\n");
}

void turing_print(turing_t * t, uint8_t note)
{
  int len = turing_get_length(t);

  printf("[ ");
  for (int i = 15; i >= 0; i--) {
    if (i == 7)
      printf(" ");
    if (len-1 == i)
      printf("|");
    if (t->shiftreg & (1 << i))
      printf("1");
    else
      printf("0");
  }
  printf(" ] -> ");
  printf("note value: %u\n", note);
}

#define SOME_CLOCKS(t) {\
  for (int i = 0; i < 16; ++i) { \
    turing_print(t, turing_clock(t)); \
  } \
}

void test_turing(void)
{
  turing_t t;

  turing_init(&t, time(0));
  turing_step_random(&t, 2);
  SOME_CLOCKS(&t);
  turing_step_length(&t, -1);
  SOME_CLOCKS(&t);
  turing_step_random(&t, -3);
  SOME_CLOCKS(&t);
  turing_step_random(&t, 2);
  SOME_CLOCKS(&t);
  turing_step_length(&t, -2);
  turing_step_range(&t, -3);
  SOME_CLOCKS(&t);
  turing_step_length(&t, -2);
  turing_step_range(&t, -1);
  turing_step_random(&t, -1);
  SOME_CLOCKS(&t);
  SOME_CLOCKS(&t);

  printf("TEST TURING OK!\n");
}

int main(void)
{
  test_ringbuffer();
  test_notemem();
  test_turing();
}

