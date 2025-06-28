#include "lr_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

typedef unsigned long long uint64;

#define LR_TREE_USE_M24_ALLOCATOR


#ifdef LR_TREE_USE_M24_ALLOCATOR

#define M24_MAX_AREAS 32
#define M24_DEFAULT_AREA_BLOCK_COUNT 4
#define M24_BLOCK_SIZE 1536

struct M24_Area {
    char* start;
    char* end;
} m24_areas[M24_MAX_AREAS] = { 0 };

#endif

char* node_name[256] = {
    "0", "++", "--", ".", "->", "(", "[","{", ")", "]", "}", "+", "-", "~", "!", "*", "&", "/", "%", "<<", ">>",
    "<", "<=", ">", ">=", "==", "!=", "^", "|", "&&", "||", "=", ",", ";", "N", "L", "\'", "\"", "I",
    "S+", "S", "E", "D", "Pident", "Uident", "Var", "C", "T", "Um", "Um[", "UmL", "PUml", "Pint", "Pfloat", "Pstring", "Pchar", "Pdma", "Pima",
    "Pcall", "Psubscript", "Pcast", "cast",
    "Pmdm", "Paddsub", "Pshift", "Pcmp", "Pcmpeq", "Pband", "Pbxor", "Pbor", "Pland", "Plor", "Passign", "P,"
};

char *lr_tree_node_type_to_string(unsigned char type) {
    return node_name[type];
}

lr_tree_node* lr_tree_node_create(unsigned char replacement, lr_tree_node* left, lr_tree_node* right) {
    lr_tree_node* node;
#ifdef LR_TREE_USE_M24_ALLOCATOR
    uint64 i, j, size;
    char* block;
    for (i = 0; i < M24_MAX_AREAS; i++) {
        if (m24_areas[i].start == NULL) break;
        for (block = m24_areas[i].start; block < m24_areas[i].end; block += M24_BLOCK_SIZE) {
            j = _tzcnt_u64(~*(uint64 *)block);
            if (j == 64) continue;
            *(uint64*)block |= 1ull << j;
            node = block + j * 24;
            goto assign_node_fields;
        }
    }
    if (i >= M24_MAX_AREAS) return NULL;
    size = M24_BLOCK_SIZE * M24_DEFAULT_AREA_BLOCK_COUNT * (1 + (i >> 2));
    block = malloc(size);
    if (block == NULL) return NULL;
    m24_areas[i].start = block;
    m24_areas[i].end = block + size;
    *(uint64*)block = 3;
    node = block + 24;
assign_node_fields:
#else
    node = malloc(sizeof(lr_tree_node));
    if (node == NULL) return NULL;
#endif
    node->type = replacement;
    node->left = left;
    node->right = right;
    return node;
}

lr_tree_node* lr_tree_node_create_terminal(unsigned char type, unsigned long long location) {
    return (lr_tree_node*)(1 | (unsigned int)type << 1 | location << 9);
}

void lr_tree_free(lr_tree_node* node) {
    uint64 i, j;
    
    if ((unsigned long long) node & 1 || node == NULL) return;
    lr_tree_free(node->left);
    lr_tree_free(node->right);
#ifdef LR_TREE_USE_M24_ALLOCATOR
    for (i = 0; i < M24_MAX_AREAS; i++) {
        if (node < m24_areas[i].start || node >= m24_areas[i].end) continue;
        j = ((char *) node - m24_areas[i].start) / 24 % 64;
        *(uint64*)m24_areas[i].start &= ~(1ull << j);
        break;
    }
#else
    free(node);
#endif
}

static void lr_tree_print_r(lr_tree_node* node, long indent) {
    long i;
    unsigned char t;
    char* s;
    if (node == NULL) return;
    for (i = 0; i < indent; i++)
        putchar(' ');

    if ((unsigned long)node & 1) {
        t = (unsigned long long) node >> 1;
        s = lr_tree_node_type_to_string(t);
        printf("TERMINAL(%s) at (%llu)\n", s, (unsigned long long) node >> 9);
    }
    else {
        t = node->type;
        s = lr_tree_node_type_to_string(t);
        printf("SYMBOL(%s) at (%llu) \n", s, (unsigned long long) node & 0xFFF);
        lr_tree_print_r(node->left, indent + 4);
        lr_tree_print_r(node->right, indent + 4);
    }
}

void lr_tree_print(lr_tree_node* node) {
    if (node == NULL) printf("Dead tree\n");
    lr_tree_print_r(node, 0);
}

unsigned char lr_tree_node_type(lr_tree_node* node) {
    return (unsigned long long) node & 1 ? (unsigned char)((unsigned long long) node >> 1) : node->type;
}
