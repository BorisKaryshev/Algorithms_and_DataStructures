#include "sort.h"
#include <stdlib.h>
#include <time.h>

#define TYPE int
#define RANGE (1000)

void *gen(const size_t i) {
    TYPE *out = malloc(sizeof(TYPE));
    *out = rand()%RANGE;
    return out;
};

int cmp(const void *a, const void *b) {
    const TYPE *x = a;
    const TYPE *y = b;
    return *x > *y;
};

void destr_free(void *p) {
    TYPE *q = p;
    free(q);
};

void destr(void *p) {};

int check(TYPE *begin, TYPE *end) {
    for(;begin != end-1; ++begin) {
        if(cmp(begin, begin+1))
            return 0;
    }
    return 1;
};

void print(array_t ar) {
    for(size_t i = 0; i < ar.size; ++i) {
        printf("%d ", ((TYPE *) ar.ptr)[i]);
    }
    putchar('\n');
};

double sort_helper(array_t ar, 
                  void (*sort)(void *, 
                               void *, 
                               size_t, 
                               int(*)(const void *, const void *))
) {
    array_t tmp = copy_array(ar);
    clock_t finish, start;
    start = clock();
    sort(tmp.ptr, tmp.ptr + tmp.type_size * (tmp.size), tmp.type_size, cmp);
    finish = clock();
    if(check(tmp.ptr, tmp.ptr + tmp.type_size * (tmp.size))) {
        destroy_array(tmp, destr);
        return difftime(finish, start);
    } else {
        destroy_array(tmp, destr);
        return -1;
    }
};

int main(void) {
    srand(time(NULL));
    array_t ar = init_array(30000, sizeof(TYPE), gen, destr_free);

    double time = sort_helper(ar, buble_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }

    time = sort_helper(ar, selection_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }

    time = sort_helper(ar, insertion_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    } 

    time = sort_helper(ar, shell_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }
    
    time = sort_helper(ar, countintg_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }

    time = sort_helper(ar, quick_sort);
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }

    destroy_array(ar, destr);
    return 0;
};