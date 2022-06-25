#include "sort.h"
#include <stdlib.h>
#include <time.h>

ARRAY_TYPE gen(const size_t i) {
    return rand()%70 + 1;
}

int cmp(const array_t ar, const size_t i, const size_t j) {
    return (ar.ptr[i]/10) % 10 < (ar.ptr[j]/10) % 10;
}

#define print(a) { \
    for(size_t i = 0; i < a.size; i++)\
        printf("%d ", a.ptr[i]); \
    putchar('\n'); \
}

void test_sort(const array_t ar, void (*sort)(array_t a, int (*is_bigger)(const array_t arr, const size_t i, const size_t j))) {
    array_t tmp = copy_array(ar);

    clock_t end, begin = clock();
    sort(tmp, cmp);
    end = clock();

    int is_sorted = 1;
    for (int i = 1; i < tmp.size; i++)
        if(cmp(tmp, i-1, i))
            is_sorted = 0;
    
    if(is_sorted)
        printf("Sorting %d elements took %.6f seconds\n", tmp.size, difftime(end, begin)/CLOCKS_PER_SEC);
    else{
        puts("ERROR: array isn`t sorted.");
        print(ar);
        print(tmp);
    }

    destroy_array(tmp);
}

int main(void) {
    size_t sz = 100;
    srand(time(NULL));
    array_t ar = init_array(sz, gen);

    

    //test_sort(ar, buble_sort);
    //test_sort(ar, selection_sort);
    test_sort(ar, insertion_sort); 
    test_sort(ar, shell_sort);
    shell_sort(ar, cmp);
    print(ar);

    destroy_array(ar);
    

    return 0;
}