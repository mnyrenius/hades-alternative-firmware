#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUM_OCTAVES    5
#define NUM_NOTES      (12 * NUM_OCTAVES) + 1
#define NUM_DAC_VALUES 4096
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define UBRR_9600B 103
#define UBRR_31250B 31

#ifdef TESTMODE
#define UBRR_CONF UBRR_9600B
#else
#define UBRR_CONF UBRR_31250B
#endif
 
typedef struct output_t {
  uint16_t cv;
  uint8_t gate;
  uint8_t updated;
} output_t;

#endif /* CONSTANTS_H */
