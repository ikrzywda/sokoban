#include "move_stack.h"

MoveStack *mv_stack_init() {
    MoveStack *stack = malloc(sizeof(MoveStack));
    stack->moves = malloc(sizeof(Move) * MOVE_STACK_DEFAULT_SIZE);
    stack->top = 0;
    stack->size = MOVE_STACK_DEFAULT_SIZE;

    return stack;
}

bool mv_stack_push(MoveStack *stack, Move move) {
    if (stack == NULL) return false;
    
    if (stack->top + 1 == stack->size) {
        stack->size += 30;
        stack->moves = realloc(stack->moves, sizeof(Move) * stack->size);
    }
    stack->top++;
    memcpy(stack->moves[stack->top], move, sizeof(Move));
    return true;
}

bool mv_stack_pop(MoveStack *stack, Move move) {
    if (stack == NULL || !stack->top) return false;
    memcpy(move, stack->moves[stack->top], sizeof(Move));
    stack->top--;
    return true;
}

bool mv_stack_free(MoveStack *stack) {
    if (stack == NULL) return false;
    free(stack->moves);
    free(stack);
    return true;
}

