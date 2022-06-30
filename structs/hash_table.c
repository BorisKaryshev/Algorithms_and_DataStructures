#include <stdlib.h>
#include <stdio.h>
#include "hash_table.h"

static p_hash_avl_tree_t create_hash_avl_tree(
    size_t length,
    HASH_AVL_TREE_TYPE (*generator)(const size_t i),
    int (*is_bigger)(const HASH_AVL_TREE_TYPE, const HASH_AVL_TREE_TYPE)
);
static void free_hash_avl_tree(p_hash_avl_tree_t p);
static void hash_avl_tree_append(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree);
static int hash_avl_tree_delete(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree);
static int hash_avl_tree_contains(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree);
static void hash_avl_tree_for_each(p_hash_avl_tree_t, void (*what_to_do)(HASH_AVL_TREE_TYPE*));

static p_hash_avl_tree_t create_hash_avl_tree(
    size_t length,
    HASH_AVL_TREE_TYPE (*generator)(const size_t i),
    int (*is_bigger)(const HASH_AVL_TREE_TYPE, const HASH_AVL_TREE_TYPE)
) {
    p_hash_avl_tree_t out = (p_hash_avl_tree_t) malloc(sizeof(struct hash_avl_tree_s));
    out->is_bigger = is_bigger;
    out->root = NULL;
    for(size_t i = 0; i < length; i++)
        hash_avl_tree_append(generator(i), out);

    return out; 
}
static void hash_avl_recursive_free(struct hash_avl_node_s* pnode) {
    if(pnode->left != NULL)
        hash_avl_recursive_free(pnode->left);
    if(pnode->right != NULL)
        hash_avl_recursive_free(pnode->right);
    free(pnode);
}
static void free_hash_avl_tree(p_hash_avl_tree_t p) {
    if(p->root != NULL)
        hash_avl_recursive_free(p->root);
    free(p);
}

static void hash_avl_null_height(struct hash_avl_node_s *p) {
        p->height = 0;
        if (p->left != NULL)
            hash_avl_null_height(p->left);
        if(p->right != NULL)
            hash_avl_null_height(p->right);
}
static void hash_avl_count_height(struct hash_avl_node_s *pnode) {
    if(pnode->left != NULL)
        hash_avl_count_height(pnode->left);
    if(pnode->right != NULL)
        hash_avl_count_height(pnode->right);

    if (pnode->right == NULL && pnode->left == NULL) {
        pnode->height = 1;
        return;
    }
    if (pnode->right == NULL) {
        pnode->height = pnode->left->height+1;
        return;
    }
    if (pnode->left == NULL) {
        pnode->height = pnode->right->height+1;
        return;
    }
    int a = pnode->right->height, b = pnode->left->height;
    pnode->height = (a)*(a>b) + (b)*(b>=a) + 1;
}
static void hash_avl_count_balance(struct hash_avl_node_s *pnode) {
        
    if (pnode->right != NULL && pnode->left != NULL)
        pnode->height = pnode->left->height - pnode->right->height;
    else if (pnode->right != NULL)
        pnode->height = -pnode->right->height;
    else if (pnode->left != NULL)
        pnode->height = pnode->left->height;
    else if (pnode->right == NULL && pnode->right == NULL) {
        pnode->height = 0;
        return;
    }
    
    if (pnode->left != NULL)
        hash_avl_count_balance(pnode->left);
    if(pnode->right != NULL)
        hash_avl_count_balance(pnode->right);

}

enum hash_avl_turn {
    RIGHT_TURN, LEFT_TURN
};
#define COUNT_BALANCE(a)     hash_avl_null_height(a);\
    hash_avl_count_height(a); \
    hash_avl_count_balance(a);

static struct hash_avl_node_s* hash_avl_rotate(struct hash_avl_node_s *pnode, enum hash_avl_turn turn_type) {
    if(turn_type == RIGHT_TURN) {
        if(pnode->left == NULL) {
            puts("ERROR: wrong turn");
            exit(-1);
        }
        struct hash_avl_node_s *tmp = pnode->left;
        while(tmp->right != NULL) 
            tmp=tmp->right;
        tmp->right = pnode;
        tmp = pnode->left;
        pnode->left = NULL;

        COUNT_BALANCE(tmp);
        return tmp;
    } else {
        if(pnode->right == NULL) {
            puts("ERROR: wrong turn");
            exit(-1);
        }

        struct hash_avl_node_s *tmp = pnode->right;
        while(tmp->left != NULL) 
            tmp=tmp->left;
        tmp->left = pnode;
        tmp = pnode->right;
        pnode->right = NULL;

        COUNT_BALANCE(tmp);
        return tmp;
    }
}

static int hash_avl_is_balanced(struct hash_avl_node_s *pnode) {
    if(pnode->right != NULL && pnode->left != NULL)
        return ((pnode->height)*(pnode->height) < 2) && hash_avl_is_balanced(pnode->left) && hash_avl_is_balanced(pnode->right); 
    if(pnode->left == NULL && pnode->right == NULL)
        return ((pnode->height)*(pnode->height)) < 2;

    if(pnode->left != NULL) 
        return ((pnode->height)*(pnode->height) < 2) && hash_avl_is_balanced(pnode->left);
    if(pnode->right != NULL) 
        return ((pnode->height)*(pnode->height) < 2) && hash_avl_is_balanced(pnode->right);
}



static void hash_avl_balance_tree(struct hash_avl_node_s *pnode) {
    if(pnode->left != NULL) {
        struct hash_avl_node_s *tmp = pnode->left;
        if ((tmp->height) * (tmp->height) > 1) {
            if (tmp->height > 0)
                pnode->left = hash_avl_rotate(tmp, RIGHT_TURN);
            else
                pnode->left = hash_avl_rotate(tmp, LEFT_TURN);
        }
    }
    if(pnode->right != NULL) {
        struct hash_avl_node_s *tmp = pnode->right;
        if ((tmp->height) * (tmp->height) > 1) {
            if (tmp->height > 0)
                pnode->right = hash_avl_rotate(tmp, RIGHT_TURN);
            else
                pnode->right = hash_avl_rotate(tmp, LEFT_TURN);
        }
    }
    if(pnode->left != NULL) 
        hash_avl_balance_tree(pnode->left);
    if(pnode->right != NULL) 
        hash_avl_balance_tree(pnode->right);
}

static void hash_avl_tree_append(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree) {
    struct hash_avl_node_s *pnode = tree->root;
    if (pnode == NULL) {
        tree->root = (struct hash_avl_node_s *) malloc(sizeof(struct hash_avl_node_s));

        tree->root->count = 1;
        tree->root->value = val;
        tree->root->right = NULL;
        tree->root->left = NULL;
        tree->root->height = 0;

        return; 
    }
    int is_new_created = 0;
    while(pnode->value != val) {
        if (tree->is_bigger(val, pnode->value)) {
            if (pnode->right == NULL) {
                pnode->right = (struct hash_avl_node_s *)malloc(sizeof(struct hash_avl_node_s));

                pnode->right->count = 0;
                pnode->right->value = val;
                pnode->right->right = NULL;
                pnode->right->left = NULL;
                pnode->right->height = 0;
                pnode = pnode->right;

                is_new_created = 1;
                break;
            }
            else
                pnode = pnode->right;
        } else {
            if (pnode->left == NULL) {
                pnode->left = (struct hash_avl_node_s *)malloc(sizeof(struct hash_avl_node_s));

                pnode->left->count = 0;
                pnode->left->value = val;
                pnode->left->right = NULL;
                pnode->left->left = NULL;
                pnode->left->height = 0;
                pnode = pnode->left;

                is_new_created = 1;
                break;
            } else
                pnode = pnode->left; 
        }
    }
    pnode->count++;
    
    if(!is_new_created)
        return;
    
    COUNT_BALANCE(tree->root);
    while(!hash_avl_is_balanced(tree->root)){
        if ((tree->root->height) * (tree->root->height) > 1) {
            if (tree->root->height > 0)
                tree->root = hash_avl_rotate(tree->root, RIGHT_TURN);
            else
                tree->root = hash_avl_rotate(tree->root, LEFT_TURN);
        }
        hash_avl_balance_tree(tree->root);
        COUNT_BALANCE(tree->root);
    }   
    
}

static int hash_avl_tree_delete(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree) {
    if (tree->root == NULL)
        return 0;
    struct hash_avl_node_s *pnode = tree->root, *tmp;
    if (tree->root->value == val) {
        pnode->count--;
        if(pnode->count > 0)
            return 1;
        while(pnode->left != NULL)
            pnode = pnode->left;
        pnode->left = tree->root->left;
        tmp = tree->root;
        tree->root = tree->root->right;
        free(tmp);
    } else {
        while(1) {
            if(pnode->value == val) {
                pnode->count--;
                if (pnode->count > 0)
                    return 1;
            }
            if (pnode->left != NULL && pnode->left->value == val) {
                tmp = pnode->left->right;
                if (tmp == NULL) {
                    tmp = pnode->left;
                    pnode->left = tmp->left;
                    free(tmp);
                    break;
                }
                while (tmp->left != NULL) 
                    tmp = tmp->left;
                tmp->left = pnode->left->left;
                tmp = pnode->left;
                pnode->left = tmp->right;
                free(tmp);

                break;
            } else if (pnode->right != NULL && pnode->right->value == val) {
                tmp = pnode->right->right;
                if(tmp == NULL) {
                    tmp = pnode->right;
                    pnode->right = tmp->left;
                    free(tmp);
                    break;
                }
                while(tmp->left != NULL)
                    tmp = tmp->left;
                tmp->left = pnode->right->left;
                tmp = pnode->right;
                pnode->right = tmp->right;
                free(tmp);
                break;
            }
            
            if(tree->is_bigger(val, pnode->value)) {
                if (pnode->right != NULL)
                    pnode = pnode->right;
                else
                    return 0;
            } else {
                if (pnode->left != NULL)
                    pnode = pnode->left;
                else
                    return 0;
            }
                
        }
    }
    COUNT_BALANCE(tree->root);
    while(!hash_avl_is_balanced(tree->root)){
        if ((tree->root->height) * (tree->root->height) > 1) {
            if (tree->root->height > 0)
                tree->root = hash_avl_rotate(tree->root, RIGHT_TURN);
            else
                tree->root = hash_avl_rotate(tree->root, LEFT_TURN);
        }
        hash_avl_balance_tree(tree->root);
        COUNT_BALANCE(tree->root);
    }   

}
static int hash_avl_tree_contains(HASH_AVL_TREE_TYPE val, p_hash_avl_tree_t tree) {
    struct hash_avl_node_s *pnode = tree->root;
    while (pnode != NULL && pnode->value != val) {
        if (tree->is_bigger(val, pnode->value))
            pnode = pnode->right;
        else
            pnode = pnode->left;
    }
    if (pnode == NULL)
        return 0;
    return 1;
}

static void hash_avl_tree_for_each(p_hash_avl_tree_t tree, void (*what_to_do)(HASH_AVL_TREE_TYPE*)) {
    void hash_avl_for_each_helper(struct hash_avl_node_s * pnode) {
        if (pnode == NULL)
            return;

        hash_avl_for_each_helper(pnode->left);
        what_to_do(&pnode->value);
        hash_avl_for_each_helper(pnode->right);
    };
    hash_avl_for_each_helper(tree->root);
}
#undef COUNT_BALANCE
#include <string.h>

static int cmp(const pdata_t a, const  pdata_t b) {
    return strcmp(a->name, b->name) > 0;
}
static pdata_t gen(const size_t i) {
    return NULL;
}

phash_table_t create_hash_table(size_t number_of_possible_elem, size_t (*hash)(const char *name, size_t size)) {
    phash_table_t out = (phash_table_t) malloc(sizeof(struct hash_table_s));
    out->hash = hash;
    out->size = number_of_possible_elem;
    out->arr = (p_hash_avl_tree_t *) malloc(number_of_possible_elem*sizeof(p_hash_avl_tree_t));
    for(size_t i = 0; i < number_of_possible_elem; i++)
        out->arr[i] = create_hash_avl_tree(0, gen, cmp);

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

/*
static void expand_table(phash_table_t ptable) {
    p_hash_avl_tree_t *tmp = (p_hash_avl_tree_t *) malloc(number_of_possible_elem*sizeof(p_hash_avl_tree_t));
}
*/
void hash_table_append(phash_table_t ptable, pdata_t pd) {
    ptable->number_of_elements++;
    /*
    if(ptable->number_of_elements > 1000*ptable->size) 
        expand_table(ptable);
    */
    hash_avl_tree_append(pd, ptable->arr[ptable->hash(pd->name, ptable->size)]);
}
int hash_table_delete(phash_table_t ptable, pdata_t pd) {
    if(hash_avl_tree_delete(pd, ptable->arr[ptable->hash(pd->name, ptable->size)])) {
        ptable->number_of_elements--;
        return 1;
    } 
    return 0;
}
HASH_TABLE_TYPE* hash_get_by_key(phash_table_t ptable, const char *name) {
    p_hash_avl_tree_t ptree = ptable->arr[ptable->hash(name, ptable->size)];
    struct hash_avl_node_s *pnode = ptree->root;
    while(1) {
        if(pnode == NULL)
            return NULL;
        if(strcmp(pnode->value->name, name) == 0)
            return &pnode->value->value;
        if(strcmp(pnode->value->name, name) > 0)
            pnode = pnode->left;
        else
            pnode=pnode->right;
    }
}

void free_hash_table(phash_table_t ptable) {
    for(size_t i = 0; i < ptable->size; i++)
        free_hash_avl_tree(ptable->arr[i]);
    free(ptable);
}