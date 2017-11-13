#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "mcp4921.h"
#include "uart.h"
#include "midi.h"
#include "settings.h"

/* ---------- PORT CONFIGURATION ----------
 *
 * PB2 - mcp4921 ss
 * PB3 - mcp4921 mosi
 * PB5 - mcp4921 sck
 *
 * PD2 - gate
 *
 *-------------------------------------- */

#define NUM_OCTAVES    5
#define NUM_NOTES      (12 * NUM_OCTAVES) + 1
#define NUM_DAC_VALUES 4096
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct mode_t {
  void (*note_on)(uint8_t, uint8_t);
  void (*note_off)(uint8_t);
};

enum mode {
  MODE_LAST_NOTE,
  MODE_MIDI_LEARN,
  MODE_END
};

static uint16_t          _dac_values[NUM_NOTES];
static struct settings_t _settings;

// modes, set by sending note on at midi channel 16

// c - note priority last played
// default
void note_on_mode_last_note(uint8_t channel, uint8_t note)
{
  if (channel == _settings.midi_channel && note < NUM_NOTES) {
    mcp4921_write(_dac_values[note]);
    PORTD |= _BV(PD2);
  }
}

void note_off_mode_last_node(uint8_t channel)
{
  if (channel == _settings.midi_channel) {
    PORTD &= ~ _BV(PD2);
  }
}

// c# - midi learn
// when active, send any note on the midi channel you want
// to use for midi to cv
void note_on_mode_midi_learn(uint8_t channel, uint8_t note)
{
  if (channel < 15) {
    _settings.midi_channel = channel;
    settings_write(&_settings);
  }
}

void note_off_mode_midi_learn(uint8_t channel)
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

struct mode_t _modes[] = {
  { .note_on = note_on_mode_last_note,  .note_off = note_off_mode_last_node  },
  { .note_on = note_on_mode_midi_learn, .note_off = note_off_mode_midi_learn },
};

static enum mode _current_mode;

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

void note_on(uint8_t channel, uint8_t note)
{
  if (channel == 15) {
    uint8_t k = note % 12;
    switch (k) {
      case 0:
        _current_mode = MODE_LAST_NOTE;
        break;
      case 1:
        _current_mode = MODE_MIDI_LEARN;
        break;
      default:
        break;
    }
  } else {
    _modes[_current_mode].note_on(channel, note);
  }
}

void note_off(uint8_t channel)
{
  _modes[_current_mode].note_off(channel);
}

int main()
{
  // setup gate as out
  DDRD |= _BV(PD2);

  generate_dac_values(_dac_values);
  _current_mode = MODE_LAST_NOTE;

  struct midi_t midi = {
    .note_on = note_on,
    .note_off = note_off,
  };

  uint8_t rxb;

  mcp4921_init();
  settings_read(&_settings);
  midi_init(&midi);
  uart_init();

  sei();

  while (1) {
    rxb = uart_receive();
    midi_process(&midi, rxb);
  }
}

