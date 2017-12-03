#include "settings.h"
#include <avr/eeprom.h>
#include <util/delay.h>

#define MAGIC 0x3c

settings_t EEMEM _eeprom_settings = {
  .midi_channel = 0,
  .mode = 0
};

uint8_t EEMEM _magic;

void settings_read(settings_t *s)
{
  if (eeprom_read_byte(&_magic) == MAGIC) {
    eeprom_read_block(s, &_eeprom_settings, sizeof(_eeprom_settings));
    if (s->midi_channel > 15)
      s->midi_channel = 0;
    if (s->mode > 15)
      s->mode = 0;
  } else {
    s->midi_channel = 0;
    s->mode = 0;
  }
}

void settings_write(settings_t *s)
{
  eeprom_update_block(s, &_eeprom_settings, sizeof(_eeprom_settings));
  eeprom_update_byte(&_magic, MAGIC);
}

