#include <stdlib.h>
#include <assert.h>
#include "stack.h"

stack_t* create_stack(size_t max) {
    stack_t *p_stack = NULL;

    p_stack = (stack_t*) malloc(sizeof(stack_t));
    assert(p_stack != NULL);

    p_stack -> max = max;
    p_stack -> top = -1;
    p_stack -> data = NULL;

    p_stack -> data = (int*) calloc(max, sizeof(int));
    assert(p_stack -> data != NULL);

    return p_stack;
}

int StackIsFull(stack_t* p_stack) {
    return (p_stack -> top >= p_stack -> max - 1);
}

int StackIsEmpty(stack_t* p_stack) {
    return (p_stack -> top <= -1);
}

int stack_push(stack_t* p_stack, int new_data) {
    if (StackIsFull(p_stack)) {
        return STACK_OVERFLOW;
    }

    p_stack -> top++;
    p_stack -> data[p_stack -> top] = new_data;

    return SUCCESS;
}

int stack_pop(stack_t* p_stack, int* old_data) {
    if (StackIsEmpty(p_stack)) {
        return STACK_UNDERFLOW;
    }

    *old_data = p_stack -> data[p_stack -> top];
    p_stack -> top--;

    return SUCCESS;
}

int StackPeek(stack_t* p_stack, int* data) {
    if (StackIsEmpty(p_stack)) {
        return STACK_EMPTY;
    }

    *data = p_stack -> data[p_stack -> top];

    return SUCCESS;
}

void StackDestroy(stack_t** pp_stack) {
    stack_t *p_stack = *pp_stack;

    free(p_stack -> data);
    free(p_stack);

    *pp_stack = NULL;
}
