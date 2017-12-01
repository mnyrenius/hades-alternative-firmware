#include "mode_midilearn.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "settings.h"
#include <util/delay.h>
 
struct mode_midilearn_t mode_midilearn = {
  .base = {
    .init = mode_midilearn_init,
    .exit = 0,
    .note_on = mode_midilearn_note_on,
    .note_on = mode_midilearn_note_off,
    .clock = 0,
    .update = 0,
  }
};

void mode_midilearn_init(struct mode_t *cxt)
{
}

void mode_midilearn_note_on(struct mode_t *cxt, uint8_t channel, uint8_t note)
{
  if (channel < 15) {
    struct mode_midilearn_t *m_cxt = (struct mode_midilearn_t*)cxt;
    m_cxt->settings->midi_channel = channel;
    settings_write(m_cxt->settings);
  }
}

void mode_midilearn_note_off(struct mode_t *cxt, uint8_t channel, uint8_t note)
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
 
