#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

struct settings_t {
  uint8_t midi_channel;
};

void settings_read(struct settings_t *s);
void settings_write(struct settings_t *s);

#endif /* SETTINGS_H */
