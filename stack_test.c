#include <stdio.h>
#include "stack.h"

int main() {
    stack_t *p_stack = NULL;
    int data;

    p_stack = create_stack(5);

    printf("P_STACK: %llu\n", p_stack);
    printf("TOP: %ld\n", p_stack -> top);
    printf("MAX: %zu\n", p_stack -> max);

    printf("FULL? %d\n", stack_is_full(p_stack)); 
    printf("EMPTY? %d\n", stack_is_empty(p_stack)); 

    for (int i = 0; i < 6; i++) {
        printf("PUSH: %d\n", stack_push(p_stack, i * 10));
        printf("PEEK: %d\n", stack_peek(p_stack, &data));
        printf("Data: %d\n", data);
    }

    printf("================================");
    printf("TOP: %ld\n", p_stack -> top);
    for (int i = 0; i < 6; i++) {
        printf("POP: %d\n", stack_pop(p_stack, &data));
        printf("Data: %d\n", data);
    }

    stack_destroy(&p_stack);
    return 0;
}
