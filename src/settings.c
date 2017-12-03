#include "settings.h"
#include <avr/eeprom.h>
#include <util/delay.h>

#define MAGIC 0x3c

settings_t EEMEM eeprom_settings;

uint8_t EEMEM magic;

void settings_read(settings_t *s)
{
  if (eeprom_read_byte(&magic) == MAGIC) {
    eeprom_read_block(s, &eeprom_settings, sizeof(eeprom_settings));
    if (s->midi_channel > 15)
      s->midi_channel = 0;
    if (s->mode > 15)
      s->mode = 0;
    if (s->midi_base_note > 195)
      s->midi_base_note = 0;
  } else {
    s->midi_channel = 0;
    s->mode = 0;
    s->midi_base_note = 0;
  }
}

void settings_write(settings_t *s)
{
  eeprom_update_block(s, &eeprom_settings, sizeof(eeprom_settings));
  eeprom_update_byte(&magic, MAGIC);
}

