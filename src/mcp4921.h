#ifndef MCP4921_H
#define MCP4921_H

#include <stdint.h>

void mcp4921_init(void);
void mcp4921_write(uint16_t value);

#endif /* MCP4921_H */
