#include "mode_turing.h"
#include "turing.h"
#include "constants.h"
#include <avr/interrupt.h>
#include "settings.h"
 
struct mode_turing_t mode_turing = {
  .base = {
    .init = (void*) mode_turing_init,
    .exit = (void*) mode_turing_exit,
    .note_on = (void*) mode_turing_note_on,
    .note_off = (void*) mode_turing_note_off,
    .clock = (void*) mode_turing_clock,
    .update = (void*) mode_turing_update,
  }
};

void mode_turing_init(struct mode_turing_t *cxt)
{
  turing_init(cxt->turing, 0);

  TCCR1B |= (1 << WGM12);
  OCR1A = 60000;
  TCCR1B |= ((1 << CS10) | (1 << CS11));
}

void mode_turing_exit(struct mode_turing_t *cxt)
{
  TCCR1B = 0;
}

void mode_turing_note_on(struct mode_turing_t *cxt, uint8_t channel, uint8_t note)
{
  if (channel == cxt->settings->midi_channel) {
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
}

void mode_turing_note_off(struct mode_turing_t *cxt, uint8_t channel, uint8_t note)
{
}

static volatile uint8_t counter = 0;
void mode_turing_clock(struct mode_turing_t *cxt)
{
  if (counter == 0) {
    uint8_t note = turing_clock(cxt->turing);
    if (note < NUM_NOTES)
      cxt->state->cv = cxt->dac_values[note];
    cxt->state->gate = 1;
    counter++;  
  } else if (counter > 0) {
    cxt->state->gate = 0;
    counter = 0;
  }
  cxt->state->updated = 1;
}
 
void mode_turing_update(struct mode_turing_t *cxt)
{
  if (TIFR1 & (1 << OCF1A)) {
    mode_turing_clock(cxt);
    TIFR1 = (1 << OCF1A);
  }
}
