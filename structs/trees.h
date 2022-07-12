#ifndef TREES
#define TREES
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/* SIMPLE BINARY TREE */

struct bin_tree_node_s {
    void  *value;
    size_t count;
    struct bin_tree_node_s *left;
    struct bin_tree_node_s *right;  
};

typedef struct bin_tree_s {
    struct bin_tree_node_s *root;
    int (*is_bigger) (const void  *, const void  *); /* if not (a > b) and not (a < b) then a == b */
}* pb_tree_t;

pb_tree_t create_bin_tree(
    size_t length, 
    void  * (*generator)(const size_t i), 
    int (*is_bigger)(const void  *, const void  *),
    void (*destroy)(void *)
);
void free_bin_tree(pb_tree_t, void (*destroy)(void *));
void bin_tree_append(pb_tree_t tree, void * value);
void bin_tree_for_each(pb_tree_t, void (*what_to_do)(void  **));
int bin_tree_delete(pb_tree_t, void  *, void (*destroy)(void *));
int bin_tree_contains(pb_tree_t, void *);

/* AVL TREE */
struct avl_node_s {
    void *value;
    size_t count;
    struct avl_node_s *left, *right;
    short height;
};

typedef struct avl_tree_s {
    struct avl_node_s *root;
    int (*is_bigger)(const void *, const void *);
}* p_avl_tree_t;

p_avl_tree_t create_avl_tree(
    size_t length,
    void * (*generator)(const size_t i),
    int (*is_bigger)(const void *, const void *),
    void (*destroy)(void *)
);
void free_avl_tree(p_avl_tree_t p, void (*destroy)(void *));
void avl_tree_append(p_avl_tree_t tree, void * val);
int avl_tree_delete(p_avl_tree_t tree, void * val, void (*destroy)(void *));
int avl_tree_contains(p_avl_tree_t tree, void * val);
void avl_tree_for_each(p_avl_tree_t, void (*what_to_do)(void **));
void avl_draw(p_avl_tree_t tree, const char *name, void (*print_node)(struct avl_node_s *, FILE *out));

#endif