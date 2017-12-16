#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "ringbuffer.h"
#include "constants.h"

static ringbuffer_t rx_buffer, tx_buffer;

void uart_init(void)
{
  ringbuffer_init(&rx_buffer);
  ringbuffer_init(&tx_buffer);

  UBRR0 = UBRR_CONF;

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
  UCSR0B = _BV(RXEN0)  | _BV (TXEN0) | _BV(RXCIE0);
}

int uart_receive(uint8_t * data)
{
  return ringbuffer_read(&rx_buffer, data);
}

void uart_transmit(uint8_t *data, int len)
{
  for (int i = 0; i < len; ++i) {
    ringbuffer_write(&tx_buffer, data[i]);
  }

  UCSR0B |= _BV(UDRIE0);
}

ISR(USART_RX_vect)
{
  char data = UDR0;
  ringbuffer_write(&rx_buffer, data);
}

ISR(USART_UDRE_vect)
{
  uint8_t data;
  int ret = ringbuffer_read(&tx_buffer, &data);
  if (ret == 0) {
    UDR0 = data;
  } else {
    UCSR0B &= ~_BV(UDRIE0);
  }
}
