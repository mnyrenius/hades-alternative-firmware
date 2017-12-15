#include "mode_turing.h"
#include "turing.h"
#include "constants.h"
#include <avr/interrupt.h>
#include "settings.h"
 
static void mode_init(mode_turing_t *cxt)
{
  turing_init(cxt->turing, 0);

  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);
  OCR1A = 60000;
  TCCR1B |= ((1 << CS10) | (1 << CS11));
}

static void mode_exit(mode_turing_t *cxt)
{
  TCCR1B = 0;
}

static void mode_note_on(mode_turing_t *cxt, uint8_t note, uint8_t channel)
{
  if (channel != cxt->settings->midi_channel) {
    return;
  }

  switch (note % 12) {
    case 0:
      turing_step_random(cxt->turing, 1);
      break;
    case 1:
      turing_step_random(cxt->turing, -1);
      break;
    case 2:
      turing_step_length(cxt->turing, 1);
      break;
    case 3:
      turing_step_length(cxt->turing, -1);
      break;
    case 5:
      turing_step_range(cxt->turing, 1);
      break;
    case 6:
      turing_step_range(cxt->turing, -1);
      break;
    default:
      break;
  }
}

static volatile uint8_t counter = 0;
static void mode_clock(mode_turing_t *cxt)
{
  if (counter == 0) {
    uint8_t note = turing_clock(cxt->turing);
    if (note < NUM_NOTES)
      cxt->out->cv = cxt->dac_values[note];
    cxt->out->gate = 1;
    counter++;  
  } else if (counter > 0) {
    cxt->out->gate = 0;
    counter = 0;
  }
  cxt->out->updated = 1;
}
 
static void mode_update(mode_turing_t *cxt)
{
  if (TIFR1 & (1 << OCF1A)) {
    mode_clock(cxt);
    TIFR1 = (1 << OCF1A);
  }
}

void mode_turing_event(mode_t *cxt, enum event ev)
{
  switch (ev) {
    case EVENT_INIT:
      mode_init(cxt->turing_cxt);
      break;
    case EVENT_NOTE_ON:
      mode_note_on(cxt->turing_cxt, cxt->note, cxt->channel);
      break;
    case EVENT_RT_CLOCK:
      mode_clock(cxt->turing_cxt);
      break;
    case EVENT_UPDATE:
      mode_update(cxt->turing_cxt);
      break;
    case EVENT_EXIT:
      mode_exit(cxt->turing_cxt);
      break;
    default:
      break;
  }
}
