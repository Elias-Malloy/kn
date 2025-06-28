#ifndef LR_TREE_H
#define LR_TREE_H

typedef struct lr_tree_node {
    unsigned char type;
    unsigned char pad[sizeof(void *) - 1]; // can be used for metadata
    struct lr_tree_node *left;
    struct lr_tree_node *right;
} lr_tree_node;

lr_tree_node *lr_tree_node_create(unsigned char type, lr_tree_node *left, lr_tree_node *right);
lr_tree_node *lr_tree_node_create_terminal(unsigned char type, unsigned long long location);
void lr_tree_free(lr_tree_node *node);
void lr_tree_print(lr_tree_node *node);
unsigned char lr_tree_node_type(lr_tree_node *node);

char *lr_tree_node_type_to_string(unsigned char type);

#endif
