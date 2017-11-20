#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "mcp4921.h"
#include "uart.h"
#include "midi.h"
#include "settings.h"
#include "notemem.h"

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
  void (*note_off)(uint8_t, uint8_t);
};

enum mode {
  MODE_NOTE_PRIO_LAST,
  MODE_NOTE_PRIO_HIGH,
  MODE_NOTE_PRIO_LOW,
  MODE_MIDI_LEARN,

  MODE_END
};

static uint16_t          _dac_values[NUM_NOTES];
static struct settings_t _settings;
static struct notemem_t  _notemem;

void note_on_mode_note_prio(uint8_t channel, uint8_t note)
{
  if (channel == _settings.midi_channel && note < NUM_NOTES) {
    uint8_t n = notemem_note_on(&_notemem, note);
    if (n < NUM_NOTES) {
      mcp4921_write(_dac_values[n]);
      PORTD |= _BV(PD2);
    }
  }
}

void note_off_mode_note_prio(uint8_t channel, uint8_t note)
{
  if (channel == _settings.midi_channel) {
    uint8_t next = notemem_note_off(&_notemem, note);
    if (next < NUM_NOTES)
      mcp4921_write(_dac_values[next]);
    else
      PORTD &= ~ _BV(PD2);
  }
}

void note_on_mode_midi_learn(uint8_t channel, uint8_t note)
{
  if (channel < 15) {
    _settings.midi_channel = channel;
    settings_write(&_settings);
  }
}

void note_off_mode_midi_learn(uint8_t channel, uint8_t note)
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
  { .note_on = note_on_mode_note_prio,  .note_off = note_off_mode_note_prio  },
  { .note_on = note_on_mode_note_prio,  .note_off = note_off_mode_note_prio  },
  { .note_on = note_on_mode_note_prio,  .note_off = note_off_mode_note_prio  },
  { .note_on = note_on_mode_midi_learn, .note_off = note_off_mode_midi_learn },
};

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
      case 0: // c - mode note prio last
        _settings.mode = MODE_NOTE_PRIO_LAST;
        notemem_init(&_notemem, NM_PRIO_LAST);
        break;
      case 1: // c# - mode midi channel learn
        _settings.mode = MODE_MIDI_LEARN;
        break;
      case 2: // d - mode note prio high
        _settings.mode = MODE_NOTE_PRIO_HIGH;
        notemem_init(&_notemem, NM_PRIO_HIGH);
        break;
      case 4: // e - mode note prio low
        _settings.mode = MODE_NOTE_PRIO_LOW;
        notemem_init(&_notemem, NM_PRIO_LOW);
        break;
      default:
        break;
    }
    settings_write(&_settings);
  } else {
    _modes[_settings.mode].note_on(channel, note);
  }
}

void note_off(uint8_t channel, uint8_t note)
{
  _modes[_settings.mode].note_off(channel, note);
}

int main()
{
  // setup gate as out
  DDRD |= _BV(PD2);

  generate_dac_values(_dac_values);
  settings_read(&_settings);

  switch (_settings.mode) {
    case MODE_NOTE_PRIO_LAST:
      notemem_init(&_notemem, NM_PRIO_LAST);
      break;
    case MODE_NOTE_PRIO_HIGH:
      notemem_init(&_notemem, NM_PRIO_HIGH);
      break;
    case MODE_NOTE_PRIO_LOW:
      notemem_init(&_notemem, NM_PRIO_LOW);
      break;
    default:
      break;
  }

  struct midi_t midi = {
    .note_on = note_on,
    .note_off = note_off,
  };

  uint8_t rxb;

  mcp4921_init();
  midi_init(&midi);
  uart_init();

  sei();

  while (1) {
    if (uart_receive(&rxb) == 0)
      midi_process(&midi, rxb);
  }
}

