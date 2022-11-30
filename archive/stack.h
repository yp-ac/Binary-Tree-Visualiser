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
    long int top, max; 
};

typedef struct stack stack_t;

stack_t* CreateStack(size_t max);

int StackIsFull(stack_t* p_stack);
int StackIsEmpty(stack_t* p_stack);

int stack_push(stack_t* p_stack, int new_data);
int stack_pop(stack_t* p_stack, int* old_data);

int StackPeek(stack_t* p_stack, int* data);
void StackDestroy(stack_t **pp_stack);

#endif
