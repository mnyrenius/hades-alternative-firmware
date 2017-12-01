#ifndef MODE_H
#define MODE_H

#include <stdint.h>

struct mode_t {
  void (*init)(struct mode_t*);
  void (*note_on)(struct mode_t*, uint8_t, uint8_t);
  void (*note_off)(struct mode_t*, uint8_t, uint8_t);
  void (*clock)(struct mode_t*);
  void (*update)(struct mode_t*);
  void (*exit)(struct mode_t*);
};

enum mode {
  MODE_NOTE_PRIO_LAST,
  MODE_NOTE_PRIO_HIGH,
  MODE_NOTE_PRIO_LOW,
  MODE_MIDI_LEARN,
  MODE_TURINGMACHINE,

  MODE_END
};
  
#endif /* MODE_H */

