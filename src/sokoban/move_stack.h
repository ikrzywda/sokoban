#ifndef MOVE_STACK_H
#define MOVE_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MOVE_STACK_DEFAULT_SIZE 100

typedef int Move[3];

typedef struct MoveStack {
    Move *moves;
    int top;
    int size;
} MoveStack;

MoveStack *mv_stack_init();
bool mv_stack_push(MoveStack *stack, Move *move);
bool mv_stack_pop(MoveStack *stack, Move *move);
bool mv_stack_free(MoveStack *stack);

#endif
