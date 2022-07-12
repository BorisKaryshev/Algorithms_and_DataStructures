#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "trees.h"
#include "hash_table.h"

#define TYPE int

void *gen(const size_t i) {
    TYPE *out = malloc(sizeof(TYPE));
    *out = rand()%10;
    return out;
}

void destr(void *x) {
    free(x);
}
int cmp(const void *a, const  void *b) {
    const TYPE *x = a, *y = b;
    return (*x) > (*y);
}

void print(void **x) {
    TYPE *y = *x;
    printf("%d ", *y);
}

#define SIZE 4

int main() {
    srand(time(NULL));
    phash_table_t table = hash_table_init();
    
    const char *names[] = {
        "first",
        "second",
        "third",
        "fourth"
    };
    
    for(int i = 0; i < SIZE; i++) {
        hash_table_append_by_name(table, names[i], gen(i));
    }
    for(int i = 0; i < SIZE; i++) {
        TYPE *p = hash_get_by_key(table, names[i]);
        printf("Name: \"%s\", value = %d\n", names[i], *p);
    }

    free_hash_table(table, destr);
    return 0;
}