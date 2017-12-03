#include "notemem.h"

static inline int cmp_hi(uint8_t a, uint8_t b)
{
  return a > b;
}

static inline int cmp_lo(uint8_t a, uint8_t b)
{
  return a < b;
}

static inline int cmp_la(uint8_t a, uint8_t b)
{
  return 1;
}

static inline void _insert(notemem_t *nm, node_t *node)
{
  node_t **n = &nm->top;

  if (!(*n)) {
    *n = node;
    return;
  }

  while((*n)->next && !nm->cmp(node->note, (*n)->note))
    n = &(*n)->next;

  if (nm->cmp(node->note, (*n)->note)) {
    node->next = *n;
    *n = node;
  } else {
    (*n)->next = node;
  }
}


static inline void _remove(notemem_t *nm, uint8_t note)
{
  node_t **n = &nm->top;

  if (note == (*n)->note) {
    (*n)->note = 0xff;
    *n = (*n)->next;
    return;
  }

  while((*n)->next && note != (*n)->next->note)
    n = &(*n)->next;

  if ((*n)->next) {
    (*n)->next->note = 0xff;
    (*n)->next = (*n)->next->next;
  }
}

void notemem_init(notemem_t *nm, enum notemem_prio prio)
{
  nm->top = 0;
  nm->active = 0xff;

  for (int i = 0; i < NUM_NODES; ++i)
    nm->nodes[i].note = 0xff;

  switch (prio) {
    case NM_PRIO_HIGH:
      nm->cmp = cmp_hi;
      break;
    case NM_PRIO_LOW:
      nm->cmp = cmp_lo;
      break;
    case NM_PRIO_LAST:
      nm->cmp = cmp_la;
      break;
    default:
      break;
  }
}

uint8_t notemem_note_on(notemem_t *nm, uint8_t note)
{
  if (nm->active != 0xff) {
    uint8_t idx = 0;
    for (; idx < NUM_NODES && nm->nodes[idx].note != 0xff; ++idx)
      if (idx == NUM_NODES - 1)
        return 0xff;

    if (nm->cmp(note, nm->active)) {
      nm->nodes[idx].note = nm->active;
      nm->active = note;
    } else
      nm->nodes[idx].note = note;

    nm->nodes[idx].next = 0;
    _insert(nm, &nm->nodes[idx]);
  } else {
    nm->active = note;
  }
  return nm->active;
}

uint8_t notemem_note_off(notemem_t *nm, uint8_t note)
{
  uint8_t res = 0xff;

  if (nm->top) {
    if (nm->active != note) {
      _remove(nm, note);
      res = nm->active;
    } else {
      nm->active = nm->top->note;
      nm->top->note = 0xff;
      res = nm->active;
      nm->top = nm->top->next;
    }
  } else {
    nm->active = 0xff;
  }

  return res;
}

