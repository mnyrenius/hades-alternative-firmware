#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"
#include "ringbuffer.h"

static struct ringbuffer_t _rb;

void uart_init(void)
{
  ringbuffer_init(&_rb);
  UBRR0 = 31;

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UCSR0B = _BV(RXEN0)  | _BV(RXCIE0);
}

int uart_receive(uint8_t * data)
{
  return ringbuffer_read(&_rb, data);
}

ISR(USART_RX_vect)
{
  char data = UDR0;
  ringbuffer_write(&_rb, data);
}
