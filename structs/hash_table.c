#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

#include <string.h>

static int cmp(const void *a, const void *b) {
    const pdata_t x = (const pdata_t) a, y = (const pdata_t) b;
    return strcmp(x->name, y->name) > 0;
}
static void * gen(const size_t i) {
    return NULL;
}

static void destr(void *pd) {
    pdata_t p = (pdata_t) pd;
    free(p->value);
    free(p->name);
    free(p);
}

phash_table_t create_hash_table(size_t number_of_possible_elem, size_t (*hash)(const char *name, size_t size)) {
    phash_table_t out = (phash_table_t) malloc(sizeof(struct hash_table_s));
    out->hash = hash;
    out->size = number_of_possible_elem;
    out->arr = (p_avl_tree_t *) malloc(number_of_possible_elem*sizeof(p_avl_tree_t));
    for(size_t i = 0; i < number_of_possible_elem; i++)
        out->arr[i] = create_avl_tree(0, gen, cmp, destr);

    return out;
}

static size_t hash(const char *s, size_t size) {
    size_t x = 0;
    for(size_t i = 0; s[i] != '\0'; i++) {
        x+=s[i];
        x*=s[i];
        x = x%size;
    }

    return x;
}

phash_table_t hash_table_init() {
    return create_hash_table(100, hash);
}


static void expand_table(phash_table_t ptable) {
    ptable->size *= 10;
    p_avl_tree_t *tmp = (p_avl_tree_t *) malloc(ptable->size * sizeof(p_avl_tree_t));
    size_t i = 0;
    void copy(void **pd) {
        pdata_t p = *pd;
        avl_tree_append(tmp[ptable->hash(p->name, ptable->size)], p);
    }

    for(; i < ptable->size/10; i++) {
        avl_tree_for_each(ptable->arr[i], copy);
        free_avl_tree(ptable->arr[i], destr);
    }
    ptable->arr = tmp;
}

void hash_table_append(phash_table_t ptable, pdata_t pd) {
    if(!avl_tree_contains(ptable->arr[ptable->hash(pd->name, ptable->size)], pd)){
        ptable->number_of_elements++;
        avl_tree_append(ptable->arr[ptable->hash(pd->name, ptable->size)], pd);
    }
    if(ptable->number_of_elements > 1000*ptable->size) 
        expand_table(ptable);
    
}

int hash_table_delete(phash_table_t ptable, pdata_t pd, void (*destroy)(void *)) {
    if(avl_tree_delete(ptable->arr[ptable->hash(pd->name, ptable->size)], pd, destroy)) {
        if (!avl_tree_contains(ptable->arr[ptable->hash(pd->name, ptable->size)], pd)){
            ptable->number_of_elements--;
        }
        return 1;
    } 
    return 0;
}
void* hash_get_by_key(phash_table_t ptable, const char *name) {
    p_avl_tree_t ptree = ptable->arr[ptable->hash(name, ptable->size)];
    struct avl_node_s *pnode = ptree->root;
    while(1) {
        if(pnode == NULL)
            return NULL;
        pdata_t val = pnode->value;
        if(strcmp(val->name, name) == 0)
            return val->value;
        if(strcmp(val->name, name) > 0)
            pnode = pnode->left;
        else
            pnode=pnode->right;
    }
}

void free_hash_table(phash_table_t ptable, void (*destroy)(void *)) {
    for(size_t i = 0; i < ptable->size; i++)
        free_avl_tree(ptable->arr[i], destroy);
    free(ptable);
}


void hash_table_append_by_name(phash_table_t ptable, const char *name, void *data) {
    pdata_t pd = malloc(sizeof(struct data_s));
    pd->name = malloc(strlen(name) * sizeof(char));
    strcpy(pd->name, name);
    pd->value = data;
    hash_table_append(ptable, pd);
};