#ifndef MODE_H
#define MODE_H

#include <stdint.h>

enum event {
  EVENT_INIT,
  EVENT_NOTE_ON,
  EVENT_NOTE_OFF,
  EVENT_RT_CLOCK,
  EVENT_RT_START,
  EVENT_RT_STOP,
  EVENT_UPDATE,
  EVENT_EXIT,

  EVENT_END,
};

enum mode {
  MODE_NOTE_PRIO_LAST,
  MODE_NOTE_PRIO_HIGH,
  MODE_NOTE_PRIO_LOW,
  MODE_MIDI_LEARN,
  MODE_TURINGMACHINE,

  MODE_END
};

typedef struct mode_t {
  void (*event)(struct mode_t*, enum event);
  uint8_t channel, note;
  union {
    struct mode_prio_t *prio_cxt;
    struct mode_midilearn_t *midilearn_cxt;
    struct mode_turing_t *turing_cxt;
  };
} mode_t;

#endif /* MODE_H */

