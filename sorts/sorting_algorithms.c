#include "sort.h"
#include "lib/trees.h"

void buble_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b)) {
    for(size_t i = ar.size; i > 0; i--) {
        for(size_t j = 0; j < i; j++) {
            if (is_bigger(ar.ptr[j], ar.ptr[j+1]))
                SWAP(ar.ptr[j], ar.ptr[j+1]);
        }
    }
}

void selection_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b)) {
    for (size_t i = 0 ; i < ar.size; i++) {
        ARRAY_TYPE min = ar.ptr[i];
        size_t index = i;
        
        for (size_t j = i + 1; j < ar.size; j++)
            if (is_bigger(ar.ptr[index], ar.ptr[j])) {
                min = ar.ptr[j];
                index = j;
            }
            SWAP(ar.ptr[i], ar.ptr[index]);
    }
}

void insertion_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b)) {
    for(size_t i = 1; i < ar.size; i++) {
        for(size_t j = i; j > 0 && is_bigger(ar.ptr[j-1], ar.ptr[j]); j--) {
            SWAP(ar.ptr[j], ar.ptr[j-1]);
        }
    }
}

void shell_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b)) {
    for(size_t step = ar.size/2; step > 0; step /=2)
        for(size_t i = step; i < ar.size; i++) 
            for(size_t j = i; j >= step && is_bigger(ar.ptr[j-step], ar.ptr[j]); j--)
                SWAP(ar.ptr[j-step], ar.ptr[j]);
}


void countintg_sort(array_t ar, int (*is_bigger)(const ARRAY_TYPE a, const ARRAY_TYPE b)) {
    if (ar.size == 0)
        return;
    int gen(const size_t i) {
        return ar.ptr[i];
    };
    pb_tree_t tree = create_bin_tree(ar.size, gen, is_bigger);
    size_t i = 0;
    void ar_from_tree(struct bin_tree_node_s *pnode) {
        if (pnode->left != NULL) 
            ar_from_tree(pnode->left);
        while(pnode->count > 0) {
            ar.ptr[i] = pnode->value;
            i++;
            pnode->count--;
        }
        if (pnode->right != NULL) 
            ar_from_tree(pnode->right);
    };
    ar_from_tree(tree->root);
    free_bin_tree(tree);
}