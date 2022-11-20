#include <stdio.h>
#include <assert.h>
#include <graphics.h>
#include "stack.h"
#include "bst.h"

#define MAX_HEIGHT  5
#define RADIUS     10
#define ROW_HEIGHT 40

stack_t* prev_positions = NULL;
char buffer[64];

void draw_node(int x, int y, int data) {
    circle(x, y, RADIUS);
    sprintf(buffer, "%d", data);
    outtextxy(x, y, buffer);
}

void display_bst_node(bst_node_t *p_bst_st, int level, int is_left) {
    if (p_bst_st == NULL) {
        return;
    }

    int x, y = level * ROW_HEIGHT, prev;
    assert(stack_peek(prev_positions, &prev) == SUCCESS);
    x = ((is_left == TRUE) ? -1 : 1) * prev;

    draw_node(x, y, p_bst_st -> data);

    stack_push(prev_positions, x);

    display_bst_node(p_bst_st -> left, level, TRUE);
    display_bst_node(p_bst_st -> right, level, FALSE);

    stack_pop(prev_positions, &prev);
}

void display_bst(bst_t* p_bst) {
    int x = getmaxx() / 2;
    int y = 1 * ROW_HEIGHT;
    int tmp;

    assert(p_bst -> p_root_node != NULL);
    draw_node(x, y, p_bst -> p_root_node -> data);

    assert(stack_push(prev_positions, x) == SUCCESS);

    display_bst_node(p_bst -> p_root_node -> left, 2, TRUE);
    display_bst_node(p_bst -> p_root_node -> right, 2, FALSE);
    
    stack_pop(prev_positions, &tmp);
}

int main() {
    int gd = DETECT, gm;
    
    prev_positions = create_stack(MAX_HEIGHT + 2);
    bst_t *p_bst = NULL;
    int data;

    p_bst = create_bst();

    do {
        printf("> ");
        scanf("%d", &data);

        bst_insert(p_bst, data);
    } while (data != -147);

    inorder_r(p_bst);

    initgraph(&gd, &gm, "C:\\Program Files (x86)\\Colorado\\cs1300\\bgi");
    display_bst(p_bst);

    stack_destroy(&prev_positions);
    bst_destroy(&p_bst);
    getch();
    closegraph();
    return 0;    
}

// int main() {
//     stack_t* p_stack = NULL;
//     p_stack = create_stack(3);
//     assert(stack_pop(p_stack) == STACK_UNDERFLOW);

// }
