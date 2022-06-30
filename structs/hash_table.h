#ifndef HASH_TABLE
#define HASH_TABLE

#define HASH_TABLE_TYPE int
typedef struct data_s {
    HASH_TABLE_TYPE value;
    char *name;
} *pdata_t;

#define HASH_AVL_TREE_TYPE pdata_t
struct hash_avl_node_s {
    HASH_AVL_TREE_TYPE value;
    size_t count;
    struct hash_avl_node_s *left, *right;
    short height;
};

typedef struct hash_avl_tree_s {
    struct hash_avl_node_s *root;
    int (*is_bigger)(const HASH_AVL_TREE_TYPE, const HASH_AVL_TREE_TYPE);
}* p_hash_avl_tree_t;

struct hash_table_s {
    p_hash_avl_tree_t  *arr;
    size_t (*hash)(const char *name, size_t size);
    size_t size, number_of_elements;
};

typedef struct hash_table_s *phash_table_t;

phash_table_t hash_table_init();
phash_table_t create_hash_table(size_t number_of_possible_elem, size_t (*hash)(const char *name, size_t size));

void hash_table_append(phash_table_t, pdata_t);
int hash_table_delete(phash_table_t, pdata_t);
HASH_TABLE_TYPE* hash_get_by_key(phash_table_t, const char *name);
void free_hash_table(phash_table_t);

#endif