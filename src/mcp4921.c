#include "mcp4921.h"
#include <avr/io.h>

void mcp4921_init(void)
{
  // setup ss, sck and mosi as outputs
  DDRB |= _BV(PB2) | _BV(PB3) | _BV(PB5);

  // enable spi: master mode, fcpu/64
  SPSR = 0;
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);

  mcp4921_write(0);
}

void mcp4921_write(uint16_t value)
{
  uint8_t config = 0x30;

  PORTB &= ~ _BV(PB2);

  SPDR = (config & 0xf0) | (0x0f & (value >> 8));
  while (!(SPSR & (1 << SPIF)));

  SPDR = value & 0x00ff;
  while (!(SPSR & (1 << SPIF)));

  PORTB |= _BV(PB2);
}
