#pragma once

#include "lr_tree.h"
#include "grammar.h"
#include "datatype.h"

void write_grammar(void);
void debug_grammar(void);
syntax_tree *parse_string(char *str);