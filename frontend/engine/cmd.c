#include "cmd.h"

//no args
void session_new(void) {
    //define_logical_volumes();
    return;
}

//4 byte lenght+name (not null terminated)
void session_load(void) {
    return;
}

//4 byte ms to define tick
void tick_set(void) {
    uint32_t ms;
    if (fread(&ms, 4, 1, stdin) == 1)
        time_ms = ms;
    return;
}
