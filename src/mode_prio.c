#include "mode_prio.h"
#include "notemem.h"
#include "constants.h"
#include "settings.h"

struct mode_prio_t mode_prio = {
  .base = {
    .init = mode_prio_init,
    .exit = 0,
    .note_on = mode_prio_note_on,
    .note_off = mode_prio_note_off,
    .clock = 0,
    .update = 0,
  }
};

void mode_prio_init(struct mode_t *cxt)
{
  struct mode_prio_t *p_cxt = (struct mode_prio_t*)cxt;
  enum notemem_prio prio = MODE_NOTE_PRIO_LAST;

  switch(p_cxt->settings->mode) {
    case MODE_NOTE_PRIO_LAST:
      prio = NM_PRIO_LAST;
      break;
    case MODE_NOTE_PRIO_HIGH:
      prio = NM_PRIO_HIGH;
      break;
    case MODE_NOTE_PRIO_LOW:
      prio = NM_PRIO_LOW;
      break;
  }

  notemem_init(p_cxt->notemem, prio);
}

void mode_prio_note_on(struct mode_t *cxt, uint8_t channel, uint8_t note)
{
  struct mode_prio_t *p_cxt = (struct mode_prio_t*)cxt;
  if (channel == p_cxt->settings->midi_channel && note < NUM_NOTES) {
    uint8_t n = notemem_note_on(p_cxt->notemem, note);
    if (n < NUM_NOTES) {
      p_cxt->state->cv = p_cxt->dac_values[n];
      p_cxt->state->gate = 1;
      p_cxt->state->updated = 1;
    }
  }
}

void mode_prio_note_off(struct mode_t *cxt, uint8_t channel, uint8_t note)
{
  struct mode_prio_t *p_cxt = (struct mode_prio_t*)cxt;
  if (channel == p_cxt->settings->midi_channel) {
    uint8_t next = notemem_note_off(p_cxt->notemem, note);
    if (next < NUM_NOTES)
      p_cxt->state->cv = p_cxt->dac_values[next];
    else
      p_cxt->state->gate = 0;
    
    p_cxt->state->updated = 1;
  } 
}

void mode_prio_update(struct mode_t *cxt)
{
}
 
