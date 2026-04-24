#ifndef CMD_H
#define CMD_H

#include <stdint.h>
#include <stdio.h>

extern uint32_t time_ms;

void session_new(void);
void session_load(void);
void tick_set(void);

#endif
