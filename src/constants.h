#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUM_OCTAVES    5
#define NUM_NOTES      (12 * NUM_OCTAVES) + 1
#define NUM_DAC_VALUES 4096
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
 
struct output_state_t {
  uint16_t cv;
  uint8_t gate;
  uint8_t updated;
};

#endif /* CONSTANTS_H */
