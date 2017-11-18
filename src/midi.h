#ifndef MIDI_H
#define MIDI_H

#include <stdint.h>

struct midi_t {
  void (*note_on)(uint8_t, uint8_t);
  void (*note_off)(uint8_t, uint8_t);
  uint8_t running_status;
  uint8_t data[2];
  uint8_t data_idx;
  uint8_t expected;
};

void midi_init(struct midi_t *m);
void midi_process(struct midi_t *m, uint8_t data);

#endif /* MIDI_H */