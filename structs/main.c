#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//#include "structs.h"
//#include "trees.h"
#include "hash_table.h"

int main() {
    srand(time(NULL));
    phash_table_t ptbl = hash_table_init();
    struct data_s data[3] = {{1, "one"}, {2, "too"}, {3, "three"}};
    hash_table_append(ptbl, &data[0]);
    hash_table_append(ptbl, &data[1]);
    hash_table_append(ptbl, &data[2]);

    for(int i = 0 ; i < 3; i++) {
        HASH_TABLE_TYPE *p;
        if((p = hash_get_by_key(ptbl, data[i].name)) != NULL)
            printf("%d\n", *p);
    }

    free_hash_table(ptbl);
    putchar('\n');
    return 0;
}