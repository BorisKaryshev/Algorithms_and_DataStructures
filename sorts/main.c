#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
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
        printf("%3d ", ((TYPE *) ar.ptr)[i]);
    }
    putchar('\n');
};

double sort_time_check_helper(array_t ar, 
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

void sort_time(array_t ar, 
                  void (*sort)(void *, 
                               void *, 
                               size_t, 
                               int(*)(const void *, const void *))
) {
    double time = sort_time_check_helper(ar, sort);
    
    if(time != -1) {
        printf("Array of %d elements sorted succesefully. It took %f seconds\n", ar.size, time/CLOCKS_PER_SEC);
    } else {
        puts("Error: array was't sorted");
    }
};

double average_time(size_t iterations, 
                    size_t size,
                    void (*sort)(void *, 
                                 void *, 
                                 size_t, 
                                 int(*)(const void *, const void *))
) {
    double time = 0;
    for(size_t i = 0; i < iterations; i++) {
        array_t ar = init_array(size, sizeof(TYPE), gen, destr_free);
        time = time * iterations + sort_time_check_helper(ar, sort);
        time /= (iterations+1);
        destroy_array(ar, destr);
    }
    return time/CLOCKS_PER_SEC;
}

int main(void) {
    srand(time(NULL));
    array_t ar = init_array(50000, sizeof(TYPE), gen, destr_free);

    sort_time(ar, buble_sort);
    sort_time(ar, insertion_sort);
    sort_time(ar, selection_sort);
    sort_time(ar, countintg_sort);
    sort_time(ar, shell_sort);
    sort_time(ar, quick_sort);
    sort_time(ar, merge_sort_array);

    destroy_array(ar, destr);
    
    /*
    for(size_t i = 0; i < 5000; i++) {
        double t = average_time(5, i+1, buble_sort);
        if(t != -1) {
            printf("%5d %f\n", i+1, t);
        } else {
            puts("Error: array was't sorted");
            return -1;
        }
    }
    */
    return 0;
};