#include "trees.h"

/* BINARY TREE */

pb_tree_t create_bin_tree(
    size_t length, 
    BIN_TREE_TYPE (*generator)(const size_t i), 
    int (*is_bigger)(const BIN_TREE_TYPE a, const BIN_TREE_TYPE b)
) {
    pb_tree_t out = (pb_tree_t) malloc(sizeof(struct bin_tree_s));
    out->is_bigger = is_bigger;
    out->root = NULL;
    if (length == 0) 
        return out;
        
    for(size_t i = 0; i < length; i++)
        bin_tree_append(generator(i), out);

    return out;
}
void recursive_free_bin_tree(struct bin_tree_node_s *pnode) {
    if(pnode->left != NULL)
        recursive_free_bin_tree(pnode->left);
    if (pnode->right != NULL)
        recursive_free_bin_tree(pnode->right);
    free(pnode);
}
void free_bin_tree(pb_tree_t p) {
    if(p->root != NULL)
        recursive_free_bin_tree(p->root);    
    free(p);
}

void bin_tree_append(BIN_TREE_TYPE value, pb_tree_t ptree) {
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
        if (pnode->value == value) {
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

void recursive_for_each(struct bin_tree_node_s *pnode, void (*what_to_do)(BIN_TREE_TYPE*)) {
    if (pnode->left != NULL)
        recursive_for_each(pnode->left, what_to_do);
    what_to_do(&(pnode->value));
    if (pnode->right != NULL)
        recursive_for_each(pnode->right, what_to_do);
}
void bin_tree_for_each(pb_tree_t ptree, void (*what_to_do)(BIN_TREE_TYPE*)) {
    if (ptree->root != NULL)
        recursive_for_each(ptree->root, what_to_do);
}

int bin_tree_delete(BIN_TREE_TYPE val, pb_tree_t ptree) {
    if (ptree->root == NULL)
        return 0;
    struct bin_tree_node_s *pnode = ptree->root, *pprev = NULL;
    int type;
    while(pnode->value != val) {
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
    }
    pnode->count--;
    if (pnode->count > 0)
        return 1;

    if (pnode->right == NULL && pprev != NULL) {
        if (type == 1) {
            pprev->right = pnode->left;
            free(pnode);
            return 1;
        } else {
            pprev->left = pnode->left;
            free(pnode);
            return 1;
        }
    } else if (pnode->right == NULL) {
        ptree->root = pnode->left;
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
        free(pnode);
        return 1;
    } else {
        if (pprev != NULL)
            pprev->left = pnode->right;
        else
            ptree->root = pnode->right;
        free(pnode);
        return 1;
    }

}

int bin_tree_contains(BIN_TREE_TYPE val, pb_tree_t ptree) {
    struct bin_tree_node_s *pnode = ptree->root;
    while(pnode != NULL && pnode->value != val) {
        if(ptree->is_bigger(val, pnode->value)) 
            pnode = pnode->right;
        else 
            pnode = pnode->left;
    }
    if (pnode == NULL) 
        return 0;
    else 
        return 1;
}