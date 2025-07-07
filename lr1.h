#ifndef EMM_LR1
#define EMM_LR1

#include "lr_tree.h"
#include "bitarray.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define SC LR1_SYMBOL_COUNT
#define TC LR1_TERMINAL_COUNT

static unsigned char lr1_grammar[SC][SC][TC] = { 0 };
//#ifdef LR1_AMBIGUOUS
//static void *context;
//#endif

static void lr1_add_production_rule(int previous, int current, int lookahead, unsigned char replacement);

static void lr1_add_production_rule_set(bit_array256 previous, bit_array256 current, bit_array256 lookahead, unsigned char replacement);

/*
    terminals is a string of terminals in the alphabet
    grammar is an allocated lr1 grammar with producation rules applied
    returns the root of the parse tree
*/
static lr_tree_node *lr1_parse_terminals(unsigned char *terminals, unsigned long len);

/*
This is a custom function the user can implement if they define LR1_AMBIGUOUS

this function is called for every node that gets added to the parse stack
the context argument is a preconfigurable part of an lr1_grammar, allowing the user to pass data.
An ambiguous language like C can be parsed with this parser and disambiguate, by tracking
definitions and identifiers in context and manually adjusting replacements.

returns a value less than 0 to indicate failure
*/
#ifdef LR1_AMBIGUOUS
static void lr1_disambiguate(lr_tree_node *node, lr_tree_node *root);
#endif



// implementations

static void lr1_add_production_rule(int previous, int current, int lookahead, unsigned char replacement)
{
    unsigned long i, j, k;
    unsigned long s0, s1, s2;
    unsigned long e0, e1, e2;

    previous  < 0 ? (s0 = 0, e0 = SC) : (s0 = previous,  e0 = previous  + 1);
    current   < 0 ? (s1 = 0, e1 = SC) : (s1 = current,   e1 = current   + 1);
    lookahead < 0 ? (s2 = 0, e2 = TC) : (s2 = lookahead, e2 = lookahead + 1);

    for (i = s0; i < e0; i++) for (j = s1; j < e1; j++) for (k = s2; k < e2; k++) 
        lr1_grammar[i][j][k] = replacement;
}

static void lr1_add_production_rule_set(bit_array256 previous, bit_array256 current, bit_array256 lookahead, unsigned char replacement)
{
    unsigned long i, j, k;
    unsigned long long tmp1[4], tmp2[4], tmp3[4];
    bit_array256 previous_to_add = tmp1, current_to_add = tmp2, lookahead_to_add = tmp3;
    for (bit_array256_copy(previous_to_add,  previous),  i = 0; (i = bit_array256_tzcnt(previous_to_add))  < SC; bit_array256_unset(previous_to_add,  i))
    for (bit_array256_copy(current_to_add,   current),   j = 0; (j = bit_array256_tzcnt(current_to_add))   < SC; bit_array256_unset(current_to_add,   j))
    for (bit_array256_copy(lookahead_to_add, lookahead), k = 0; (k = bit_array256_tzcnt(lookahead_to_add)) < TC; bit_array256_unset(lookahead_to_add, k))
        lr1_grammar[i][j][k] = replacement; 
}

// printf("%s %s (%s) -> %s\n", node_name[i], node_name[j], node_name[k], node_name[replacement]);

#define PARSE_STACK_SIZE 512
lr_tree_node *lr1_parse_terminals(unsigned char *terminals, unsigned long len) {
    unsigned char previous, current, lookahead, replacement;
    unsigned long i;
    lr_tree_node *node, *stack[PARSE_STACK_SIZE];
    int top = 1;

    stack[0] = lr_tree_node_create_terminal(0, 0);
    stack[1] = stack[0]; 

    for (i = 0; i < len;) {
        lookahead = terminals[i];
#ifdef LR1_EPSILON
        // optionally enable a terminal that is never placed on the parse stack
        // useful for maintaining 1-1 indexing with source code to terminals
        if (lookahead == LR1_EPSILON) { i++; continue; };
#endif
        previous = lr_tree_node_type(stack[top - 1]); 
        current  = lr_tree_node_type(stack[top]); 
        replacement = lr1_grammar[previous][current][lookahead];
        if (replacement && previous) {
            node = lr_tree_node_create(replacement, stack[top-1], stack[top]);
            if (node == NULL) goto free_parse_stack;
            top--;
            if (top == 0) goto free_parse_stack;
        } else {
            // check for single replacement
            replacement = lr1_grammar[0][current][lookahead];
            if (replacement) {
                node = lr_tree_node_create(replacement, stack[top], NULL);
                if (node == NULL) goto free_parse_stack;
            } else {
                if (top >= PARSE_STACK_SIZE - 2) goto free_parse_stack;
                // terminal tree nodes are stored in the pointer value with their source index
                // bit 1 is set to mark the value as a terminal, since pointers to nodes will be multiples of 8
                node = lr_tree_node_create_terminal(lookahead, i);
                top++;
                i++;
            }
        }
        stack[top] = node;
//        printf("%s %s (%s) -> \n", node_name[previous], node_name[current], node_name[lookahead]);
//        lr_tree_print(node);
#ifdef LR1_AMBIGUOUS
        lr1_disambiguate(node, stack[2]);
#endif
    }
    if (top == 3) return stack[2];

free_parse_stack:
    for (; top; top--) {
        lr_tree_print(stack[top]);
        lr_tree_free(stack[top]);
    }
    return NULL;
}

#endif
