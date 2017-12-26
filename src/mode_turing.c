#include "mode_turing.h"
#include "turing.h"
#include "constants.h"
#include "settings.h"
 
static void mode_init(mode_turing_t *cxt)
{
  turing_init(cxt->turing, 0);
  cxt->running = 0;
  cxt->clk_count = 0;
}

static void mode_note_on(mode_turing_t *cxt, uint8_t note)
{
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

static void mode_clock(mode_turing_t *cxt)
{
  if (!cxt->running) {
    return;
  }

  if (cxt->clk_count == 0 || cxt->clk_count == 12) {
    uint8_t note = turing_clock(cxt->turing);
    if (note < NUM_NOTES)
      cxt->out->cv = cxt->dac_values[note];
    cxt->out->gate = 1;
    cxt->out->updated = 1;
  } else if (cxt->clk_count == 6 || cxt->clk_count == 18) {
    cxt->out->gate = 0;
    cxt->out->updated = 1;
  }
 
  if (cxt->clk_count++ >= 23) {
    cxt->clk_count = 0;
  }
}

void mode_turing_event(mode_t *cxt, enum event ev)
{
  switch (ev) {
    case EVENT_INIT:
      mode_init(cxt->turing_cxt);
      break;
    case EVENT_NOTE_ON:
      mode_note_on(cxt->turing_cxt, cxt->note);
      break;
    case EVENT_RT_CLOCK:
      mode_clock(cxt->turing_cxt);
      break;
    case EVENT_RT_START:
      cxt->turing_cxt->clk_count = 0;
      cxt->turing_cxt->running = 1;
      break;
    case EVENT_RT_STOP:
      cxt->turing_cxt->running = 0;
      break;
    default:
      break;
  }
}
