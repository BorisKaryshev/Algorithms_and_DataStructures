#include "sort.h"
#include <stdlib.h>
#include <string.h>
#include "lib/trees.h"
#include "lib/structs.h"

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
    char *op = begin;
    for(char *i = op; i != end; i += type_size) {
        if(is_bigger(op, i)) {
            char *p = op + type_size;
            swap(p, i, type_size);
            swap(p, op, type_size);
            op += type_size;
            i -= type_size;
        }
    }    
    quick_sort(begin, op, type_size, is_bigger);
    quick_sort(op + type_size, end, type_size, is_bigger);
};

static void* merge(char *first_begin, 
                   char *second_begin, 
                   char *first_end, 
                   char *second_end, 
                   size_t type_size,
                   int (*is_bigger)(const void*, const void *)
) {
    void *const out = malloc((first_end - first_begin) + (second_end - second_begin));
    if(!out) {
        return NULL;
    }
    void *const out_end = ((char*)out) + (first_end - first_begin) + (second_end - second_begin);
    char *first_iter = first_begin, *second_iter = second_begin;
    char *i = out;
    for(; i != out_end && first_iter != first_end && second_iter != second_end; i += type_size) {
        if(is_bigger(first_iter, second_iter)) {
            swap(i, second_iter, type_size);
            second_iter += type_size;
        } else {
            swap(i, first_iter, type_size);
            first_iter += type_size;
        }
    }
    for(; first_iter != first_end && i != out_end; i += type_size) {
        swap(i, first_iter, type_size);
        first_iter += type_size;
    }
    for(; second_iter != second_end && i != out_end; i += type_size) {
        swap(i, second_iter, type_size);
        second_iter += type_size;
    }

    return out;
};

static void nothing_to_do(void *) {};
static void destroy(void *p) {
    destroy_array(*((array_t*)p), NULL);
    free(p);
};


static void *merge_sort_base(void* (*get_value)(const size_t i), 
                             const size_t type_size,
                             const size_t length, 
                             int (*is_bigger)(const void *, const void *)
) {
    stack_pt prev = create_stack(0, NULL);

    size_t size = 1, amount = 0;

    char *left = malloc(type_size*size), *left_end = left + type_size*size;
    char *left_iter = left;
    char *right = malloc(type_size * size), *right_end = right + type_size*size;
    char *right_iter = right;
    while(amount < length) {
        if(left_iter != left_end) {
            swap(get_value(amount++), left_iter, type_size);
            left_iter += type_size;
        } else if(right_iter != right_end) {
            swap(get_value(amount++), right_iter, type_size);
            right_iter += type_size;
        } else  {
            array_t* p_ar = stack_pop(prev);
            if(p_ar == NULL || p_ar->size != 2) {
                if(p_ar != NULL) {
                    stack_push(prev, p_ar);
                }
                p_ar = malloc(sizeof(array_t));
                p_ar->ptr = merge(left, right, left_end, right_end, type_size, is_bigger);
                p_ar->size = 2;
                p_ar->type_size = type_size;

                stack_push(prev, p_ar);

                size = 1;
                left_iter = left;
                right_iter = right;
            } else {
                void *tmp = merge(left, right, left_end, right_end, type_size, is_bigger);
                size *= 2;
                free(left);
                left = tmp;
                while(p_ar != NULL && p_ar->size == size) {
                    tmp = merge(p_ar->ptr, left, ((char*) p_ar->ptr) + size*type_size, ((char*) left) + size*type_size, type_size, is_bigger);
                    free(left);
                    size *= 2;
                    destroy_array(*p_ar, nothing_to_do);
                    free(p_ar);
                    left = tmp;
                    p_ar = stack_pop(prev);
                }
                stack_push(prev, p_ar);
                p_ar = malloc(sizeof(array_t));
                p_ar->ptr = left;
                p_ar->size = size;
                p_ar->type_size = type_size;
                stack_push(prev, p_ar);
                
                size = 1;
                left = malloc(type_size * size);
                left_iter = left;
                left_end = ((char*)left) + type_size*size;

                right_iter = right;
            }
        }
        
    }
    if(right_iter == right_end) {
        void *tmp = merge(left, right, left_end, right_end, type_size, is_bigger);
        free(left);
        left_end = ((char*)tmp) + (left_end - left) + (right_end - right); 
        left = tmp;
    } else if(right_iter != right_end && right_iter != right) {
        void *tmp = merge(left, right, left_end, right_iter + type_size, type_size, is_bigger);
        free(left);
        left_end = ((char*)tmp) + (left_end - left) + (right_end - right); 
        left = tmp;
    } else if(right_iter != right) {
        left_end = left_iter;
    }
    array_t *p_ar;
    while((p_ar = stack_pop(prev)) != NULL) {
        void *tmp = merge(p_ar->ptr, left, ((char*)p_ar->ptr) + type_size * p_ar->size, left_end, type_size, is_bigger);
        left_end = ((char*)tmp) + (left_end - left) + (p_ar->size*type_size);
        free(left);
        destroy_array(*p_ar, nothing_to_do);
        free(p_ar);
        left = tmp;
    }

    free(right);
    free_stack(prev, destroy);
    return left; 
};

void merge_sort_array(void *begin, void *end, size_t type_size, 
                      int (*is_bigger)(const void *a, const void *b)
) {
    void *gen_helper(const size_t i) {
        if(i > ((char*)end )- ((char*)begin))
            exit(-1);
        return ((char*) begin) + type_size*i;
    };
    size_t size = ((char*) end ) - ((char*)begin);
    size /= type_size;
    char *out = merge_sort_base(gen_helper, type_size, size, is_bigger);
    for(size_t i = 0; i < size; i++) {
        swap(((char*)begin) + i *type_size, out + i*type_size, type_size);
    }
    free(out);
};