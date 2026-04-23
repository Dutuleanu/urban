#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "time.h"

/* ── dispatch ── */
typedef void (*handler_fn)(void);
static void cmd_noop(void) { }
static handler_fn dispatch_table[] = {
    cmd_noop,
};
static const size_t N_COMMANDS = sizeof(dispatch_table) / sizeof(dispatch_table[0]);

/* ── main ── */
int main(void) {
    while (true) {
        __auto_type start = get_now();
        uint64_t cmd;
        while (fread(&cmd, 8, 1, stdin) == 1)
            if (cmd < N_COMMANDS)
                dispatch_table[cmd]();
        sleep_until(start);
    }
}
