#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

#ifndef TRUE
#define TRUE    1 
#define FALSE   0 
#endif

#ifndef SUCCESS
#define SUCCESS         1
#endif
#define STACK_EMPTY     2
#define STACK_OVERFLOW  3
#define STACK_UNDERFLOW 4

struct stack {
    int *data;
    size_t top, max; 
};

typedef struct stack stack_t;

stack_t* create_stack(size_t max);

int stack_is_full(stack_t* p_stack);
int stack_is_empty(stack_t* p_stack);

int stack_push(stack_t* p_stack, int new_data);
int stack_pop(stack_t* p_stack, int* old_data);

int stack_peek(stack_t* p_stack, int* data);
void stack_destroy(stack_t **pp_stack);

#endif
