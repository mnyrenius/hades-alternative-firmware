#include <stdio.h>
#include <assert.h>
#include "ringbuffer.h"
#include "notemem.h"

void test_ringbuffer(void)
{
  struct ringbuffer_t rb;
  ringbuffer_init(&rb);

  for (int i = 1; i < 32; ++i) {
    ringbuffer_write(&rb, i);
  }

  for (int i = 1; i < 32; ++i) {
    assert(ringbuffer_read(&rb) == i);
  }

  ringbuffer_write(&rb, 1);
  ringbuffer_write(&rb, 3);
  assert(ringbuffer_read(&rb) == 1);
  ringbuffer_write(&rb, 5);
  assert(ringbuffer_read(&rb) == 3);
  assert(ringbuffer_read(&rb) == 5);

  printf("TEST RINGBUFFER OK!\n");
}

void notemem_print(struct notemem_t *nm)
{
  printf("(%u, [", nm->active);
  for (struct node_t *n = nm->top; n; n = n->next) {
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
  struct notemem_t nm;
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

int main(void)
{
  test_ringbuffer();
  test_notemem();
}

