#ifndef TREES
#define TREES
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/* BINARY TREE */

#define BIN_TREE_TYPE long long
struct bin_tree_node_s {
    BIN_TREE_TYPE value;
    size_t count;
    struct bin_tree_node_s *left;
    struct bin_tree_node_s *right;  
};

typedef struct bin_tree_s {
    struct bin_tree_node_s *root;
    int (*is_bigger) (const BIN_TREE_TYPE a, const BIN_TREE_TYPE b);
}* pb_tree_t;

pb_tree_t create_bin_tree(
    size_t length, 
    BIN_TREE_TYPE (*generator)(const size_t i), 
    int (*is_bigger)(const BIN_TREE_TYPE a, const BIN_TREE_TYPE b)
);
void free_bin_tree(pb_tree_t);
void bin_tree_append(BIN_TREE_TYPE, pb_tree_t);
void bin_tree_for_each(pb_tree_t, void (*what_to_do)(BIN_TREE_TYPE*));
int bin_tree_delete(BIN_TREE_TYPE, pb_tree_t);
int bin_tree_contains(BIN_TREE_TYPE, pb_tree_t);

#endif