#include "mode_prio.h"
#include "notemem.h"
#include "constants.h"
#include "settings.h"

void mode_init(mode_prio_t *cxt)
{
  enum notemem_prio prio = MODE_NOTE_PRIO_LAST;

  switch(cxt->settings->mode) {
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

  notemem_init(cxt->notemem, prio);
}

void mode_note_on(mode_prio_t *cxt, uint8_t note)
{
  if (note < NUM_NOTES) {
    uint8_t n = notemem_note_on(cxt->notemem, note);
    if (n < NUM_NOTES) {
      cxt->out->cv = cxt->dac_values[n];
      cxt->out->gate = 1;
      cxt->out->updated = 1;
    }
  }
}

void mode_note_off(mode_prio_t *cxt, uint8_t note)
{
  uint8_t next = notemem_note_off(cxt->notemem, note);
  if (next < NUM_NOTES)
    cxt->out->cv = cxt->dac_values[next];
  else
    cxt->out->gate = 0;

  cxt->out->updated = 1;
}

void mode_prio_event(mode_t *cxt, enum event ev)
{
  switch (ev) {
    case EVENT_INIT:
      mode_init(cxt->prio_cxt);
      break;
    case EVENT_NOTE_ON:
      mode_note_on(cxt->prio_cxt, cxt->note);
      break;
    case EVENT_NOTE_OFF:
      mode_note_off(cxt->prio_cxt, cxt->note);
      break;
    default:
      break;
  };
}
