#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
//#include "trees.h"
//#include "hash_table.h"

#define TYPE int

void* gen(const size_t i) {
    TYPE *x = malloc(sizeof(TYPE));
    *x = rand()%10 + (i)*10;
    return x;
}
void destr(void *x) {
    int *y = x;
    free(y);
}
void print(void *x) {
    TYPE *y = x;
    printf("%d ", *y);
}


int main() {
    srand(time(NULL));
    plist_t list = create_list(30, gen);

    list_for_each(list, print);
    putchar('\n');

    free_list(list, destr);
    return 0;
}