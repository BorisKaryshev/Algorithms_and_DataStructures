#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include "lib/trees.h"

void swap(void *x, void *y, size_t size) {
    char *tmp = malloc(size);
    memcpy(tmp, x, size);
    memcpy(x, y, size);
    memcpy(y, tmp, size);
    free(tmp);
}

void buble_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    for(char *i = begin; i != end; i += type_size) {
        for(char *j = i+type_size; j != end; j += type_size) {
            if(is_bigger(i, j)) {
                swap(i, j, type_size);
            }
        }
    }
};



void selection_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    for(char *i = begin; i != end; i += type_size) {
        void *min = i;
        for(char *j = i; j != end; j += type_size) {
            if(is_bigger(min, j))
                min = j;
        }
        swap(min, i, type_size);
    }
};


void insertion_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    for(char *i = begin + type_size; i != end; i += type_size) {
        for(char *j = i; j != begin && is_bigger(j - type_size, j); j -= type_size) {
            swap(j - type_size, j, type_size);
        }
    }
};


void shell_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    for(size_t step = (end-begin)/(type_size*2); step > 0; step /= 2) {
        for(char *i = begin + type_size * step; i != end; i += type_size) {
            for(char *j = i; j != begin + type_size*(step-1) && is_bigger(j-type_size*step, j); j -= type_size) {
                    swap(j, j - type_size*step, type_size);
            }
        }
    }

};


void countintg_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    if(begin == end)
        return;
    void *gen(const size_t i) {
        void *out = malloc(type_size);
        swap(out, begin + i*type_size, type_size);
        return out;
    };
    void destr(void *p) {
        free(p);
    };

    pb_tree_t tree = create_bin_tree((end-begin)/type_size, gen, is_bigger, destr);
    
    char *i = begin;
    void ar_from_tree(struct bin_tree_node_s *pnode) {
        if(pnode->left) {
            ar_from_tree(pnode->left);
        }
        while(pnode->count > 0) {
            memcpy(i, pnode->value, type_size);
            i += type_size;
            pnode->count--;
        }

        if(pnode->right) {
            ar_from_tree(pnode->right);
        }
    };
    ar_from_tree(tree->root);
    free_bin_tree(tree, destr);
};

void quick_sort(void *begin, void *end, size_t type_size, 
                int (*is_bigger)(const void *a, const void *b)
) {
    if(((char *) end) - ((char *) begin) <= type_size) {
        return;
    }
    char *op = begin;// + (end - begin - type_size);
    for(char *i = begin; i != op; i += type_size) {
        if (is_bigger(i, op)) {
            char *p = op - type_size;
            swap(p, i, type_size);
            swap(p, op, type_size);
            op -= type_size;
            i -= type_size;
        }
    }
    for(char *i = op; i != end; i += type_size) {
        if(is_bigger(op, i)) {
            char *p = op + type_size;
            swap(p, i, type_size);
            swap(p, op, type_size);
            op += type_size;
            i -= type_size;
        }
    }    

    //for(char *i = begin; i != end; i += type_size) {
    //    printf("%d ", *i);
    //}
    //putchar('\n');
    quick_sort(begin, op, type_size, is_bigger);
    quick_sort(op + type_size, end, type_size, is_bigger);
};