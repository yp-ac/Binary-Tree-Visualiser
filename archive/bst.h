#ifndef _BST_H_
#define _BST_H_

#include <stdlib.h>

#ifndef TRUE
#define TRUE    1 
#define FALSE   0 
#endif

#ifndef SUCCESS
#define SUCCESS             1 
#endif

#define TREE_EMPTY          2 
#define TREE_DATA_NOT_FOUND 3 
#define TREE_NO_SUCCESSOR   4 
#define TREE_NO_PREDECESSOR 5

struct bst_node
{
    int data; 
    struct bst_node* left; 
    struct bst_node* right; 
    struct bst_node* parent; 
}; 

struct bst {
    struct bst_node* p_root_node;
    size_t nr_elements;
};

typedef struct bst_node bst_node_t;
typedef struct bst bst_t;

struct bst* create_bst(void); 
int bst_insert(struct bst* p_bst, int new_element); 

void preorder_r(struct bst* p_bst); 
void inorder_r(struct bst* p_bst); 
void postorder_r(struct bst* p_bst); 
int bst_max(struct bst* p_bst, int* p_max_data); 
int bst_min(struct bst* p_bst, int* p_min_data); 
int bst_inorder_successor(struct bst* p_bst, int ext_data, int* p_succ_data); 
int bst_inorder_predecessor(struct bst* p_bst, int ext_data, int* p_pred_data); 
int bst_search(struct bst* p_bst, int search_data); 
int remove_data(struct bst* p_nst, int r_data); 
void bst_destroy(struct bst** pp_bst); 

struct bst_node* get_max_node(struct bst_node* p_node); 
struct bst_node* get_min_node(struct bst_node* p_node); 
struct bst_node* inorder_successor(struct bst_node* p_node); 
struct bst_node* inorder_predecessor(struct bst_node* p_node); 
struct bst_node* search_node(struct bst* p_bst, int search_data); 
void preorder_node(struct bst_node* p_root_node); 
void inorder_node(struct bst_node* p_root_node); 
void postorder_node(struct bst_node* p_root_node); 
void destroy_node(struct bst_node* p_root_node); 
struct bst_node* get_bst_node(int new_element); 

#endif
