#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint64_t size;
    uint64_t ptr;
    uint64_t id;
} logical_volume_header;

typedef struct __attribute__((packed)) {
    int32_t x;
    int32_t y;
} position;

typedef struct __attribute__((packed)) {
    double money;
    position view_1;
    position view_2;
    position map;
} game_state_structure;

typedef struct __attribute__((packed)) {
    uint8_t x;
    uint8_t y;

} building_size;

typedef struct __attribute__((packed)) {
    uint64_t id;
} building_action;

#endif
