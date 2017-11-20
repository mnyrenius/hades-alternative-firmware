#ifndef NOTEMEM_H
#define NOTEMEM_H

#include <stdint.h>

/* data structure for keeping a sorted list
   of multiple simultaneously active notes.

   each new note is inserted in the correct
   position according to the prio order.
   see test_notemem for information about
   each prio.

   inspired by "note stack" implemented in
   cvpal by mutable instruments, although this
   implementation is most likely crappier :) */

#define NUM_NODES 12

struct node_t {
  uint8_t note;
  struct node_t *next;
};

struct notemem_t {
  struct node_t nodes[NUM_NODES];
  struct node_t *top;
  int (*cmp)(uint8_t, uint8_t);
  uint8_t active;
};

enum notemem_prio {
  NM_PRIO_HIGH,
  NM_PRIO_LOW,
  NM_PRIO_LAST,

  NM_PRIO_ENUM_END,
};

void notemem_init(struct notemem_t *nm, enum notemem_prio prio);
uint8_t notemem_note_on(struct notemem_t *nm, uint8_t note);
uint8_t notemem_note_off(struct notemem_t *nm, uint8_t note);

#endif /* NOTEMEM_H */
