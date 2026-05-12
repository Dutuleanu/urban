#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "time.h"
#include "runtime.h"
#include "cmd.h"
#include "io.h"
#include "mem.h"
#include "bda.h"
#include "types.h"


/* ── globals ── */
uint8_t *base_ptr, *delta_ptr, *req_ptr, *temp_ptr;

logical_volume_header *lv;

uint64_t lv_count=0;

game_state_structure *game_state;

uint64_t building_count = 0;
uint64_t marine_building_count = 0;

/* ── dispatch ── */
typedef void (*handler_fn)(void);

static handler_fn dispatch_table[] = {
    session_new,
    session_load,
    tick_set,
    dump_to_file,
    tile_build,
    tile_clear,
    viewbox_register,
};
static const size_t N_COMMANDS = sizeof(dispatch_table) / sizeof(dispatch_table[0]);

/* ── main ── */
int main(void) {

    temp_ptr = aligned_calloc(4096, 64);
    if (!temp_ptr) {
        return 1;
    }

    if (read_file_from_exe_dir("data/lv_count.bda", temp_ptr, 4096) <= 0) return 1;

    bda_descriptor bda;
    bda.x=1;
    bda.y=1;
    bda.n=1;
    bda.dest=(uint64_t)(&lv_count);
    bda.src=(uint64_t)(temp_ptr);
    bda_parse(&bda);

    memset(temp_ptr, 0, 4096);
    if (read_file_from_exe_dir("data/building_count.bda", temp_ptr, 4096) <= 0) return 1;

    bda.dest=(uint64_t)(&building_count);
    bda_parse(&bda);

    memset(temp_ptr, 0, 4096);
    if (read_file_from_exe_dir("data/marine_building_count.bda", temp_ptr, 4096) <= 0) return 1;

    bda.dest=(uint64_t)(&marine_building_count);
    bda_parse(&bda);

    base_ptr=aligned_calloc(128*1024, 64);
    *(uint64_t *)base_ptr=128*1024;
    delta_ptr=aligned_calloc(128*1024, 64);
    *(uint64_t *)delta_ptr=128*1024;
    req_ptr=aligned_calloc(128*1024, 64);
    *(uint64_t *)req_ptr=128*1024;

    while (true) {
        __auto_type start = get_now();

        while (stdin_bytes_available() >= 8) {
            uint64_t cmd;
            if (fread(&cmd, 8, 1, stdin) == 1)
                if (cmd < N_COMMANDS)
                    dispatch_table[cmd]();
        }



        update();
        delta();
        fflush(stdout);
        sleep_until(start);
    }
}
