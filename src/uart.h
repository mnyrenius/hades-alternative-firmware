#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
uint8_t uart_receive(void);

#endif /* UART_H */
