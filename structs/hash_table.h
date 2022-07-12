#ifndef HASH_TABLE
#define HASH_TABLE
#include "trees.h"

/* Hash Table */
typedef struct data_s {
    void *value;
    char *name;
} *pdata_t;


struct hash_table_s {
    p_avl_tree_t  *arr;
    size_t (*hash)(const char *name, size_t size);
    size_t size, number_of_elements;
};

typedef struct hash_table_s *phash_table_t;

phash_table_t hash_table_init();
phash_table_t create_hash_table(size_t number_of_possible_elem, 
                                size_t (*hash)(const char *name, size_t number_of_possible_elemnts) 
) ;

void hash_table_append(phash_table_t, pdata_t);
void hash_table_append_by_name(phash_table_t, const char *name, void *data);
int hash_table_delete(phash_table_t, pdata_t, void (*destroy)(void *));
int hash_table_delete_by_name(phash_table_t, const char *name, void (*destroy)(void *)); 
/* destroy receives pdata_t as input */
void *hash_get_by_key(phash_table_t, const char *name);
void free_hash_table(phash_table_t, void (*destroy)(void *)); /* destroy receives pdata_t as input */


#endif