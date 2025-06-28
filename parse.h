#ifndef PARSE_H
#define PARSE_H

#include "lr_tree.h"
#include "grammar.h"

void write_grammar(void);
void debug_grammar(void);
lr_tree_node *parse_string(char *str);

#endif
