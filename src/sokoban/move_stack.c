#include "move_stack.h"


MoveStack *mv_stack_init() {
    MoveStack *stack = malloc(sizeof(MoveStack));
    stack->moves = malloc(sizeof(int) * MOVE_STACK_DEFAULT_SIZE);
    stack->top = 0;
    stack->size = MOVE_STACK_DEFAULT_SIZE;

    return stack;
}

bool mv_stack_push(MoveStack *stack, int move) {
    if (stack == NULL) return false;
    
    if (stack->top + 1 == stack->size) {
        stack->size += 30;
        stack->moves = realloc(stack->moves, sizeof(int) * stack->size);
    }
    stack->top++;
    stack->moves[stack->top] = move;
    return true;
}

bool mv_stack_pop(MoveStack *stack, int *move) {
    if (stack == NULL || !stack->top) return false;
    *move = stack->moves[stack->top];
    stack->top--;
    printf("top = %d ", stack->top);
    return true;
}

bool mv_stack_free(MoveStack *stack) {
    if (stack == NULL) return false;
    free(stack->moves);
    free(stack);
    return true;
}

void mv_stack_print(MoveStack *stack) {
    if (stack == NULL) return;

    for (int i = 0; i <= stack->top; ++i) {
        printf("%d", stack->moves[i]);
        putchar('\n');
    }
}   
