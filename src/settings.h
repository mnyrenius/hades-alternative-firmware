#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

typedef struct settings_t {
  uint8_t midi_channel;
  uint8_t mode;
} settings_t;

void settings_read(settings_t *s);
void settings_write(settings_t *s);

#endif /* SETTINGS_H */
