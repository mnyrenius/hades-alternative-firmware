#include "mode_midilearn.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "settings.h"
#include <util/delay.h>

static void mode_note_on(mode_midilearn_t *cxt, uint8_t channel, uint8_t note)
{
  if (channel < 15) {
    cxt->settings->midi_channel = channel;
    settings_write(cxt->settings);
  }
}

static void mode_note_off(mode_midilearn_t *cxt, uint8_t note)
{
  cli();
  for (uint8_t i = 0; i < 3; ++i) {
    PORTD |= _BV(PD2);
    _delay_ms(250);
    PORTD &= ~ _BV(PD2);
    _delay_ms(250);
  }
  sei();
} 
void mode_midilearn_event(mode_t *cxt, enum event ev)
{
  switch (ev) {
    case EVENT_NOTE_ON:
      mode_note_on(cxt->midilearn_cxt, cxt->channel, cxt->note);
      break;
    case EVENT_NOTE_OFF:
      mode_note_off(cxt->midilearn_cxt, cxt->note);
      break;
    default:
      break;
  }
}
 
