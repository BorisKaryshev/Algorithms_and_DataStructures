#include "trees.h"
/* BINARY TREE */

pb_tree_t create_bin_tree(
    size_t length, 
    void * (*generator)(const size_t i), 
    int (*is_bigger)(const void *, const void *),
    void (*destroy)(void *)
) {
    pb_tree_t out = (pb_tree_t) malloc(sizeof(struct bin_tree_s));
    out->is_bigger = is_bigger;
    out->root = NULL;
    if (length == 0) 
        return out;
        
    for(size_t i = 0; i < length; i++) {
        void *elem = generator(i);
        int exists = bin_tree_contains(out, elem);
        bin_tree_append(out, elem);
        if(exists)
            destroy(elem);
    
    }
    return out;
}

void recursive_free_bin_tree(struct bin_tree_node_s *pnode, void (*destroy)(void *)) {
    if(pnode == NULL)
        return;
    recursive_free_bin_tree(pnode->left, destroy);
    recursive_free_bin_tree(pnode->right, destroy);
    destroy(pnode->value);
    free(pnode);
}

void free_bin_tree(pb_tree_t p, void (*destroy)(void *)) {
    if(p->root != NULL)
        recursive_free_bin_tree(p->root, destroy);    
    free(p);
}

void bin_tree_append(pb_tree_t ptree, void *value) {
    if (ptree->root == NULL) {
        ptree->root = (struct bin_tree_node_s *) malloc(sizeof(struct bin_tree_node_s));
        
        ptree->root->value = value;
        ptree->root->count = 1;
        ptree->root->left = NULL;
        ptree->root->right = NULL;
        
        return;
    }
    struct bin_tree_node_s *pnode = ptree->root;
    while(1) {
        if (!ptree->is_bigger(value, pnode->value) && !ptree->is_bigger(pnode->value, value)) {
            pnode->count++;
            return;
        }
        if (ptree->is_bigger(value, pnode->value)) {
            if(pnode->right == NULL) {
                pnode->right = (struct bin_tree_node_s *) malloc(sizeof(struct bin_tree_node_s));
                
                pnode = pnode->right;
                pnode->value = value;
                pnode->count = 1;
                pnode->right = NULL;
                pnode->left = NULL;
                
                return;
            }
            pnode = pnode->right; 
        } else {
            
            if(pnode->left == NULL) {
                pnode->left = (struct bin_tree_node_s *) malloc(sizeof(struct bin_tree_node_s));
                
                pnode = pnode->left;
                pnode->value = value;
                pnode->count = 1;
                pnode->right = NULL;
                pnode->left = NULL;
                
                return;
            }
            pnode = pnode->left; 
        }
    }
}


void bin_tree_for_each(pb_tree_t ptree, void (*what_to_do)(void **)) {
    void bin_recursive_for_each(struct bin_tree_node_s *pnode) {
        if (pnode->left != NULL)
            bin_recursive_for_each(pnode->left);
        what_to_do(&pnode->value);
        if (pnode->right != NULL)
            bin_recursive_for_each(pnode->right);
    }
    
    if (ptree->root != NULL)
        bin_recursive_for_each(ptree->root);
}

int bin_tree_delete(pb_tree_t ptree, void *val, void (*destroy)(void *)) {
    if (ptree->root == NULL)
        return 0;
    struct bin_tree_node_s *pnode = ptree->root, *pprev = NULL;
    int type, is_equal = !(ptree->is_bigger(val, pnode->value) || ptree->is_bigger(pnode->value, val));
    while(!is_equal) {
        if (ptree->is_bigger(val, pnode->value)) {
            if (pnode->right != NULL) {
                pprev = pnode;
                pnode = pnode->right;
                type = 1;
            } else 
                return 0;
        } else {
            if (pnode->left != NULL) {
                pprev = pnode;
                pnode = pnode->left;
                type = 2;
            } else 
                return 0;
        }
    is_equal = !(ptree->is_bigger(val, pnode->value) || ptree->is_bigger(pnode->value, val));
    }
    pnode->count--;
    if (pnode->count > 0)
        return 1;

    if (pnode->right == NULL && pprev != NULL) {
        if (type == 1) {
            pprev->right = pnode->left;
            destroy(pnode->value);
            free(pnode);
            return 1;
        } else {
            pprev->left = pnode->left;
            destroy(pnode->value);
            free(pnode);
            return 1;
        }
    } else if (pnode->right == NULL) {
        ptree->root = pnode->left;
        destroy(pnode->value);
        free(pnode);
        return 1;
    }

    struct bin_tree_node_s *tmp = pnode->right;
    while(tmp->left != NULL) {
        tmp = tmp->left;
    }
    tmp->left = pnode->left;
    if (type == 1) {
        if (pprev != NULL)
            pprev->right = pnode->right;
        else
            ptree->root = pnode->right;
        destroy(pnode->value);
        free(pnode);
        return 1;
    } else {
        if (pprev != NULL)
            pprev->left = pnode->right;
        else
            ptree->root = pnode->right;
        destroy(pnode->value);
        free(pnode);
        return 1;
    }

}

int bin_tree_contains(pb_tree_t ptree, void *value) {
    struct bin_tree_node_s *pnode = ptree->root;
    while(pnode != NULL) {
        if (!ptree->is_bigger(value, pnode->value) && !ptree->is_bigger(pnode->value, value)) {
            return 1;
        }
        if(ptree->is_bigger(value, pnode->value)) 
            pnode = pnode->right;
        else 
            pnode = pnode->left;
    }
    return 0;
}

/* AVL TREE */

p_avl_tree_t create_avl_tree(
    size_t length,
    void * (*generator)(const size_t i),
    int (*is_bigger)(const void *, const void *),
    void (*destroy)(void *)
) {
    p_avl_tree_t out = (p_avl_tree_t) malloc(sizeof(struct avl_tree_s));
    out->is_bigger = is_bigger;
    out->root = NULL;
    for(size_t i = 0; i < length; i++) {
        void *elem = generator(i);
        int contains = avl_tree_contains(out, elem);
        avl_tree_append(out, elem);
        if (contains) {
            destroy(elem);
        }
    }

    return out; 
}

void free_avl_tree(p_avl_tree_t p, void (*destroy)(void *)) {
    void avl_recursive_free(struct avl_node_s* pnode) {
        if(pnode->left != NULL)
            avl_recursive_free(pnode->left);
        if(pnode->right != NULL)
            avl_recursive_free(pnode->right);
        destroy(pnode->value);
        free(pnode);
    }
    if(p->root != NULL)
        avl_recursive_free(p->root);
    free(p);
}

static void avl_null_height(struct avl_node_s *p) {
        p->height = 0;
        if (p->left != NULL)
            avl_null_height(p->left);
        if(p->right != NULL)
            avl_null_height(p->right);
}
static void avl_count_height(struct avl_node_s *pnode) {
    if(pnode->left != NULL)
        avl_count_height(pnode->left);
    if(pnode->right != NULL)
        avl_count_height(pnode->right);

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
static void avl_count_balance(struct avl_node_s *pnode) {
        
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
        avl_count_balance(pnode->left);
    if(pnode->right != NULL)
        avl_count_balance(pnode->right);

}

enum avl_turn {
    RIGHT_TURN, LEFT_TURN
};
#define COUNT_BALANCE(a)     avl_null_height(a);\
    avl_count_height(a); \
    avl_count_balance(a);

static struct avl_node_s* avl_rotate(struct avl_node_s *pnode, enum avl_turn turn_type) {
    if(turn_type == RIGHT_TURN) {
        if(pnode->left == NULL) {
            puts("ERROR: wrong turn");
            exit(-1);
        }
        struct avl_node_s *tmp = pnode->left;
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

        struct avl_node_s *tmp = pnode->right;
        while(tmp->left != NULL) 
            tmp=tmp->left;
        tmp->left = pnode;
        tmp = pnode->right;
        pnode->right = NULL;

        COUNT_BALANCE(tmp);
        return tmp;
    }
}

static int avl_is_balanced(struct avl_node_s *pnode) {
    if(pnode->right != NULL && pnode->left != NULL)
        return ((pnode->height)*(pnode->height) < 2) && avl_is_balanced(pnode->left) && avl_is_balanced(pnode->right); 
    if(pnode->left == NULL && pnode->right == NULL)
        return ((pnode->height)*(pnode->height)) < 2;

    if(pnode->left != NULL) 
        return ((pnode->height)*(pnode->height) < 2) && avl_is_balanced(pnode->left);
    if(pnode->right != NULL) 
        return ((pnode->height)*(pnode->height) < 2) && avl_is_balanced(pnode->right);
}



static void avl_balance_tree(struct avl_node_s *pnode) {
    if(pnode->left != NULL) {
        struct avl_node_s *tmp = pnode->left;
        if ((tmp->height) * (tmp->height) > 1) {
            if (tmp->height > 0)
                pnode->left = avl_rotate(tmp, RIGHT_TURN);
            else
                pnode->left = avl_rotate(tmp, LEFT_TURN);
        }
    }
    if(pnode->right != NULL) {
        struct avl_node_s *tmp = pnode->right;
        if ((tmp->height) * (tmp->height) > 1) {
            if (tmp->height > 0)
                pnode->right = avl_rotate(tmp, RIGHT_TURN);
            else
                pnode->right = avl_rotate(tmp, LEFT_TURN);
        }
    }
    if(pnode->left != NULL) 
        avl_balance_tree(pnode->left);
    if(pnode->right != NULL) 
        avl_balance_tree(pnode->right);
}

void avl_tree_append(p_avl_tree_t tree, void * val) {
    struct avl_node_s *pnode = tree->root;
    if (pnode == NULL) {
        tree->root = (struct avl_node_s *) malloc(sizeof(struct avl_node_s));

        tree->root->count = 1;
        tree->root->value = val;
        tree->root->right = NULL;
        tree->root->left = NULL;
        tree->root->height = 0;

        return; 
    }
    int is_new_created = 0;
    while(tree->is_bigger(val, pnode->value) || tree->is_bigger(pnode->value, val)) {
        if (tree->is_bigger(val, pnode->value)) {
            if (pnode->right == NULL) {
                pnode->right = (struct avl_node_s *)malloc(sizeof(struct avl_node_s));

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
                pnode->left = (struct avl_node_s *)malloc(sizeof(struct avl_node_s));

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
    while(!avl_is_balanced(tree->root)){
        if ((tree->root->height) * (tree->root->height) > 1) {
            if (tree->root->height > 0)
                tree->root = avl_rotate(tree->root, RIGHT_TURN);
            else
                tree->root = avl_rotate(tree->root, LEFT_TURN);
        }
        avl_balance_tree(tree->root);
        COUNT_BALANCE(tree->root);
    }   
    
}

#include <string.h>

void avl_draw(p_avl_tree_t tree, const char *name, void (*print_node)(struct avl_node_s *, FILE *out)) {


    FILE *out = fopen("output.dot", "w");
    fputs("graph name {\n", out);    

    void avl_draw_helper(struct avl_node_s *pnode) {
        print_node(pnode, out);
        if (pnode->left != NULL)
            avl_draw_helper(pnode->left);
        if (pnode->right != NULL)
            avl_draw_helper(pnode->right);
    }
    
    avl_draw_helper(tree->root);

    fputs("}", out);
    fclose(out);
    char outname[1000] = "rm ";
    strcat(outname, name);
    system(outname);
    outname[0] = '\0';
    strcat(outname, "dot -Tsvg output.dot -o ./");
    strcat(outname, name);
    
    system(outname);
    system("rm output.dot");
}

int avl_tree_delete(p_avl_tree_t tree, void * val, void (*destroy)(void *)) {
    if (tree->root == NULL)
        return 0;
    struct avl_node_s *pnode = tree->root, *tmp;
    if (!tree->is_bigger(pnode->value, val) && !tree->is_bigger(val, pnode->value)) {
        pnode->count--;
        if(pnode->count > 0) {
            return 1;
        }
        if (pnode->right == NULL) {
            tmp = pnode;
            tree->root = pnode->left;
        } else {
            tmp = pnode->right;
            while(tmp->left != NULL) {
                tmp = tmp->left;
            }
            tmp->left = pnode->left;
            tree->root = pnode->right;
        }
    } else {
        tmp = pnode;
        int type, x;
        while((x = tree->is_bigger(val, pnode->value) - tree->is_bigger(pnode->value, val)) != 0) {
            type = x;
            switch (x) {
                case 1:
                    if(pnode->right == NULL) {
                        return 0;
                    }
                    tmp = pnode;
                    pnode = pnode->right;
                break;
                case -1:
                    if(pnode->left == NULL) {
                        return 0;
                    }
                    tmp = pnode;
                    pnode = pnode->left;
                    break;
            }
        }
        pnode->count--;
        if(pnode->count > 0) {
            return 1;
        }
        if(pnode->right == NULL) {
            if (type > 0) {
                tmp->right = pnode->left;
            }
            else {
                tmp->left = pnode->left;
            }
        } else {
            struct avl_node_s *node = pnode->right;
            for(; node->left != NULL; node = node->left);
            node->left = pnode->left;
            if (type > 0)
                tmp->right = pnode->right;
            else 
                tmp->left = pnode->right;
                
        }

    }
    destroy(pnode->value);
    free(pnode);

    COUNT_BALANCE(tree->root);
    while(!avl_is_balanced(tree->root)){
        if ((tree->root->height) * (tree->root->height) > 1) {
            if (tree->root->height > 0)
                tree->root = avl_rotate(tree->root, RIGHT_TURN);
            else
                tree->root = avl_rotate(tree->root, LEFT_TURN);
        }
        avl_balance_tree(tree->root);
        COUNT_BALANCE(tree->root);
    }   

}
int avl_tree_contains(p_avl_tree_t tree, void * val) {
    struct avl_node_s *pnode = tree->root;
    while (pnode != NULL) {
        if (!tree->is_bigger(val, pnode->value) && !tree->is_bigger(pnode->value, val)) {
            return 1;
        }
        if (tree->is_bigger(val, pnode->value))
            pnode = pnode->right;
        else
            pnode = pnode->left;
    }
    return 0;
}

void avl_tree_for_each(p_avl_tree_t tree, void (*what_to_do)(void **)) {
    void avl_for_each_helper(struct avl_node_s * pnode) {
        if (pnode == NULL)
            return;

        avl_for_each_helper(pnode->left);
        what_to_do(&pnode->value);
        avl_for_each_helper(pnode->right);
    };
    avl_for_each_helper(tree->root);
}

#undef COUNT_BALANCE