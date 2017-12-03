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

typedef struct node_t {
  uint8_t note;
  struct node_t *next;
} node_t;

typedef struct notemem_t {
  node_t nodes[NUM_NODES];
  node_t *top;
  int (*cmp)(uint8_t, uint8_t);
  uint8_t active;
} notemem_t;

enum notemem_prio {
  NM_PRIO_HIGH,
  NM_PRIO_LOW,
  NM_PRIO_LAST,

  NM_PRIO_ENUM_END,
};

void notemem_init(notemem_t *nm, enum notemem_prio prio);
uint8_t notemem_note_on(notemem_t *nm, uint8_t note);
uint8_t notemem_note_off(notemem_t *nm, uint8_t note);

#endif /* NOTEMEM_H */
