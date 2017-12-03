#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "mcp4921.h"
#include "uart.h"
#include "midi.h"
#include "settings.h"
#include "notemem.h"
#include "turing.h"
#include "mode.h"
#include "mode_prio.h"
#include "mode_midilearn.h"
#include "mode_turing.h"
#include "constants.h"

/* ---------- PORT CONFIGURATION ----------
 *
 * PB2 - mcp4921 ss
 * PB3 - mcp4921 mosi
 * PB5 - mcp4921 sck
 *
 * PD2 - gate
 *
 *-------------------------------------- */

typedef struct hades_t {
  settings_t settings;
  notemem_t notemem;
  turing_t turing;
  uint16_t dac_values[NUM_NOTES];
  mode_t modes[MODE_END];
  output_t out;
} hades_t;

void generate_dac_values(uint16_t *values)
{
  // v_out = v_ref * d / 4096
  // 1 volt is ~= 820 which gives 820 / 12 = 68.333.. per
  // note. since we need an integer, multiply by 3 -> 205 and
  // use this as the step between each note.
  // finally divide each note value by 3 to get the real dac value again and it
  // should hopefully be quite close to 1v/oct.
  for (uint8_t i = 0; i < NUM_NOTES; ++i)
    values[i] = MIN(NUM_DAC_VALUES - 1, 205 * i / 3);
}

void note_on(void *arg, uint8_t channel, uint8_t note)
{
  hades_t * cxt = (hades_t *)arg;
  mode_t *m = &cxt->modes[cxt->settings.mode];

  if (channel == 15) {
    m->event(m, EVENT_EXIT);
    uint8_t k = note % 12;
    switch (k) {
      case 0: // c - mode note prio last
        cxt->settings.mode = MODE_NOTE_PRIO_LAST;
        break;
      case 1: // c# - mode midi channel learn
        cxt->settings.mode = MODE_MIDI_LEARN;
        break;
      case 2: // d - mode note prio high
        cxt->settings.mode = MODE_NOTE_PRIO_HIGH;
        break;
      case 4: // e - mode note prio low
        cxt->settings.mode = MODE_NOTE_PRIO_LOW;
        break;
      case 5: // f = mode turing machine
        cxt->settings.mode = MODE_TURINGMACHINE;
        break;
      default:
        break;
    }
    m = &cxt->modes[cxt->settings.mode];
    m->event(m, EVENT_INIT);
    settings_write(&cxt->settings);
  } else {
    m->channel = channel;
    m->note = note;
    m->event(m, EVENT_NOTE_ON);
  }
}

void note_off(void *arg, uint8_t channel, uint8_t note)
{
  hades_t * cxt = (hades_t *)arg;
  mode_t *m = &cxt->modes[cxt->settings.mode];
  m->channel = channel;
  m->note = note;
  m->event(m, EVENT_NOTE_OFF);
}

void clock(void *arg)
{
  hades_t * cxt = (hades_t *)arg;
  mode_t *m = &cxt->modes[cxt->settings.mode];
  m->event(m, EVENT_RT_CLOCK);
}

void start(void *cxt)
{
}

void stop(void *cxt)
{
}

int main()
{
  DDRD |= _BV(PD2);

 hades_t hades = {
    .out = {
      .cv = 0,
      .gate = 0,
      .updated = 0,
    },
  };

  mode_prio_t mode_prio = {
    .settings = &hades.settings,
    .notemem = &hades.notemem,
    .dac_values = hades.dac_values,
    .out = &hades.out,
  };

  mode_midilearn_t mode_midilearn = {
    .settings = &hades.settings,
  };

  mode_turing_t mode_turing = {
    .settings = &hades.settings,
    .turing = &hades.turing,
    .dac_values = hades.dac_values,
    .out = &hades.out,
  };

  hades.modes[MODE_NOTE_PRIO_LAST] = (mode_t) { .event = mode_prio_event       , .prio_cxt      = &mode_prio      };
  hades.modes[MODE_NOTE_PRIO_HIGH] = hades.modes[MODE_NOTE_PRIO_LAST];
  hades.modes[MODE_NOTE_PRIO_LOW]  = hades.modes[MODE_NOTE_PRIO_LAST];
  hades.modes[MODE_MIDI_LEARN]     = (mode_t) { .event = mode_midilearn_event  , .midilearn_cxt = &mode_midilearn };
  hades.modes[MODE_TURINGMACHINE]  = (mode_t) { .event = mode_turing_event     , .turing_cxt    = &mode_turing    };

  generate_dac_values(hades.dac_values);
  settings_read(&hades.settings);

  mode_t *m = &hades.modes[hades.settings.mode];
  m->event(m, EVENT_INIT);

  midi_t midi = {
    .callback_data = &hades,
    .note_on = note_on,
    .note_off = note_off,
    .rt_clock = clock,
    .rt_start = start,
    .rt_stop = stop,
  };

  uint8_t rxb;

  mcp4921_init();
  midi_init(&midi);
  uart_init();

  sei();

  while (1) {
    if (uart_receive(&rxb) == 0)
      midi_process(&midi, rxb);

    m = &hades.modes[hades.settings.mode];
    m->event(m, EVENT_UPDATE);

    if (hades.out.updated) {
      mcp4921_write(hades.out.cv);
      if (hades.out.gate)
        PORTD |= _BV(PD2);
      else
        PORTD &= ~_BV(PD2);

      hades.out.updated = 0;
    }
  }
}

