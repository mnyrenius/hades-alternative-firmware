#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
int uart_receive(uint8_t *data);

#endif /* UART_H */
