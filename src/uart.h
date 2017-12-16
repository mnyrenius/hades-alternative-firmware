#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
int uart_receive(uint8_t *data);
void uart_transmit(uint8_t *data, int len);

#endif /* UART_H */
