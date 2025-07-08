#include "grammar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LR1_EPSILON 255
#define LR1_TERMINAL_COUNT TERMINAL_COUNT
#define LR1_SYMBOL_COUNT SYMBOL_COUNT

//#define LR1_AMBIGUOUS
// disambiguate is disabled until more of its features are complete
// if re-enabled, integer constants will be evaluated

#include "lr1.h"
#include "datatype.h"

static void string_to_terminals(char* str, unsigned char* terminals, unsigned long len);

char* src_code;

#define MAX_IDENTIFIER_LEN 256

static void lr1_disambiguate(lr_tree_node *node, lr_tree_node *root) {
    unsigned char type, ltype, *name;
    unsigned long long i, n, scale, len;
    lr_tree_node* subnode, *auxnode;
	syntax_tree_variable *var;
	data_type *datatype;
    char identifier[MAX_IDENTIFIER_LEN];
	
	type = lr_tree_node_type(node);
    switch (type) {
		case SYMBOL_DECLARATION:
			// test for name multiple inclusion in same list
			datatype = node->left;
			
			
		case SYMBOL_IDENTIFIER:
			len = 0;
			subnode = node->right;
			do {
				identifier[len++] = src_code[(uint64_t) subnode->right >> 9];
				subnode = subnode->left;
			} while (subnode);
			identifier[len] = '\0';
			// test if identifier is predefined keyword
			if (strcmp(identifier, "if") == 0) {
				// replace keyword etc
			} else {
				// lookup previous variable definition
				subnode = root;
				do {
					type = lr_tree_node_type(subnode);
					if (type != SYMBOL_STATEMENTS) {
						node->type = SYMBOL_UNKNOWN_IDENTIFIER;
						break;
					}
					auxnode = subnode->right->right;
					if (auxnode->type == SYMBOL_DECLARATION) {
						auxnode = auxnode->right;
						do {
							var = auxnode->right;
							if (strcmp(identifier, var->name) == 0) {								
								memcpy(node, var, sizeof(syntax_tree_variable));
								return;
							}
						} while (auxnode = auxnode->left);
					}
					subnode = subnode->left;
				} while (1);
				node->type = SYMBOL_UNKNOWN_IDENTIFIER;
			}
			break;
        case SYMBOL_CONSTANT:
            ltype = lr_tree_node_type(node->left);
            //rtype = lr_tree_node_type(node->right);
            if (ltype == SYMBOL_PARTIAL_INT_LITERAL) {
                n = 0;
                scale = 1;
                subnode = node;
                
                while (subnode->right) {
                    n += scale * (src_code[(unsigned long long) subnode->right >> 9] - '0');
                    scale *= 10;
                    subnode = subnode->left;
                }
                if ((unsigned long long) subnode->left % 2 == 0) subnode = subnode->left;
                n += scale * (src_code[(unsigned long long) subnode->left >> 9] - '0');
                lr_tree_free(node->left);
                node->pad[0] = n & 1;
                node->left = (void*) (n | 1);
                // node->right = n;
                //printf("parsed constant int: %llu\n", n);
            }
            else if (ltype == SYMBOL_PARTIAL_FLOAT_LITERAL) {

            }
            else if (ltype == SYMBOL_PARTIAL_STRING_LITERAL) {

            }
            else {
                
            }
            break;
        default:
            return;
    }
}

lr_tree_node* parse_string(char* str) {
    unsigned char* terminals;
    unsigned long len;
    src_code = str;
    len = strlen(str);
    terminals = malloc(len+1);
    if (terminals == NULL) return NULL;
    terminals[len] = 0;
    string_to_terminals(str, terminals, len);
    lr_tree_node* parse_tree = lr1_parse_terminals(terminals, len + 1);
    free(terminals);
    lr_tree_print(parse_tree);
    return parse_tree;
}

void write_grammar(void) {
    unsigned long long mem[28] = { 0 }, i, j; 
    bit_array256 expression = mem, all = mem + 4, suffix_operator = mem + 8, prefix_operator = mem + 12;
    bit_array256 not_higher_precedence_operator = mem + 16, operator_set = mem + 20, partial_binary_operator = mem + 24;

    bit_array256_not(all);
    int any = -1;
// STATEMENTS
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_END_STATEMENT, any, SYMBOL_STATEMENT);
    lr1_add_production_rule(0, SYMBOL_STATEMENT, any, SYMBOL_STATEMENTS);
    lr1_add_production_rule(SYMBOL_STATEMENTS, SYMBOL_STATEMENT, any, SYMBOL_STATEMENTS);
// IDENTIFIERS

	lr1_add_production_rule(0, TERMINAL_LETTER, any, SYMBOL_PARTIAL_IDENTIFIER);
	lr1_add_production_rule(SYMBOL_PARTIAL_IDENTIFIER, TERMINAL_LETTER, any, SYMBOL_IDENTIFIER);
	lr1_add_production_rule(0, SYMBOL_PARTIAL_IDENTIFIER, any, SYMBOL_IDENTIFIER);
	lr1_add_production_rule(0, SYMBOL_PARTIAL_IDENTIFIER, TERMINAL_LETTER, TERMINAL_NONE);
	// on replacement, SYMBOL_IDENTIFIER is looked up and replaced by disambiguate

	// grab the identifier early if it is the name of a struct field
	// this way the identifier is looked up exclusively against possible field names for the type of struct
	lr1_add_production_rule(SYMBOL_PARTIAL_INDIRECT_ACCESS, SYMBOL_PARTIAL_IDENTIFIER, any, SYMBOL_EXPRESSION);
	lr1_add_production_rule(SYMBOL_PARTIAL_INDIRECT_ACCESS, SYMBOL_PARTIAL_IDENTIFIER, TERMINAL_LETTER, TERMINAL_NONE);
	
	lr1_add_production_rule(SYMBOL_PARTIAL_DIRECT_ACCESS, SYMBOL_PARTIAL_IDENTIFIER, any, SYMBOL_EXPRESSION);
	lr1_add_production_rule(SYMBOL_PARTIAL_DIRECT_ACCESS, SYMBOL_PARTIAL_IDENTIFIER, TERMINAL_LETTER, TERMINAL_NONE);

// TYPES
    // the implied word after undefined is identifier
    lr1_add_production_rule(SYMBOL_TYPENAME, SYMBOL_MODIFIED_UNDEFINED_LIST, TERMINAL_END_STATEMENT, SYMBOL_DECLARATION); 
    lr1_add_production_rule(SYMBOL_TYPENAME, SYMBOL_MODIFIED_UNDEFINED_LIST, TERMINAL_CLOSE_PAREN, SYMBOL_DECLARATION); 
        
    lr1_add_production_rule(SYMBOL_MODIFIED_UNDEFINED_LIST, TERMINAL_COMMA, any, SYMBOL_PARTIAL_MODIFIED_UNDEFINED_LIST); 
    lr1_add_production_rule(SYMBOL_PARTIAL_MODIFIED_UNDEFINED_LIST, SYMBOL_MODIFIED_UNDEFINED, any, SYMBOL_MODIFIED_UNDEFINED_LIST);
	lr1_add_production_rule(0, SYMBOL_MODIFIED_UNDEFINED, any, SYMBOL_MODIFIED_UNDEFINED_LIST);

    lr1_add_production_rule(SYMBOL_MODIFIED_UNDEFINED, TERMINAL_OPEN_BRACKET, any, SYMBOL_MODIFIED_UNDEFINED_SUBSCRIPT);
    lr1_add_production_rule(SYMBOL_MODIFIED_UNDEFINED_SUBSCRIPT, TERMINAL_CLOSE_BRACKET, any, SYMBOL_MODIFIED_UNDEFINED);
    lr1_add_production_rule(SYMBOL_MODIFIED_UNDEFINED_SUBSCRIPT, SYMBOL_EXPRESSION, TERMINAL_CLOSE_BRACKET, SYMBOL_MODIFIED_UNDEFINED_SUBSCRIPT);

	lr1_add_production_rule(0, SYMBOL_UNKNOWN_IDENTIFIER, any, SYMBOL_MODIFIED_UNDEFINED);
	lr1_add_production_rule(TERMINAL_STAR, SYMBOL_MODIFIED_UNDEFINED, any, SYMBOL_MODIFIED_UNDEFINED);
	

// LITERALS
    lr1_add_production_rule(0, TERMINAL_NUMBER, any, SYMBOL_PARTIAL_INT_LITERAL);
    lr1_add_production_rule(0, SYMBOL_PARTIAL_INT_LITERAL, any, SYMBOL_CONSTANT);
    lr1_add_production_rule(0, SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_NUMBER, 0);
    lr1_add_production_rule(0, SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_DOT, 0);
    lr1_add_production_rule(0, SYMBOL_PARTIAL_FLOAT_LITERAL, any, SYMBOL_CONSTANT);
    lr1_add_production_rule(0, SYMBOL_PARTIAL_FLOAT_LITERAL, TERMINAL_NUMBER, 0);
    lr1_add_production_rule(0, SYMBOL_CONSTANT, any, SYMBOL_EXPRESSION);
    lr1_add_production_rule(SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_NUMBER, any, SYMBOL_CONSTANT);
    lr1_add_production_rule(SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_NUMBER, TERMINAL_NUMBER, SYMBOL_PARTIAL_INT_LITERAL);
    lr1_add_production_rule(SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_DOT, any, SYMBOL_PARTIAL_FLOAT_LITERAL);
    lr1_add_production_rule(SYMBOL_PARTIAL_INT_LITERAL, TERMINAL_DOT, TERMINAL_NUMBER, SYMBOL_PARTIAL_FLOAT_LITERAL);
    lr1_add_production_rule(SYMBOL_PARTIAL_FLOAT_LITERAL, TERMINAL_NUMBER, any, SYMBOL_PARTIAL_FLOAT_LITERAL);
    lr1_add_production_rule(TERMINAL_DOT, TERMINAL_NUMBER, any, SYMBOL_PARTIAL_FLOAT_LITERAL);
// OPERATORS
    bit_array256_set(suffix_operator, TERMINAL_INC);
    bit_array256_set(suffix_operator, TERMINAL_DEC);
    bit_array256_set(suffix_operator, TERMINAL_DOT);
    bit_array256_set(suffix_operator, TERMINAL_ARROW);
    bit_array256_set(suffix_operator, TERMINAL_OPEN_PAREN);
    bit_array256_set(suffix_operator, TERMINAL_OPEN_BRACKET);

    bit_array256_set(expression, SYMBOL_EXPRESSION);

    lr1_add_production_rule_set(expression, suffix_operator, all, SYMBOL_EXPRESSION);

    bit_array256_not(not_higher_precedence_operator);
    bit_array256_xor(not_higher_precedence_operator, suffix_operator);
    bit_array256_set(not_higher_precedence_operator, TERMINAL_NONE);

    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_INC, any, SYMBOL_EXPRESSION);
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_DEC, any, SYMBOL_EXPRESSION);
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_DOT, any, SYMBOL_PARTIAL_DIRECT_ACCESS);
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_ARROW, any, SYMBOL_PARTIAL_INDIRECT_ACCESS);
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_OPEN_PAREN, any, SYMBOL_PARTIAL_FUNCTION_CALL);
    lr1_add_production_rule(SYMBOL_EXPRESSION, TERMINAL_OPEN_BRACKET, any, SYMBOL_PARTIAL_SUBSCRIPT);

    bit_array256_set(prefix_operator, TERMINAL_INC);
    bit_array256_set(prefix_operator, TERMINAL_DEC);
    bit_array256_set(prefix_operator, TERMINAL_PLUS);
    bit_array256_set(prefix_operator, TERMINAL_MINUS);
    bit_array256_set(prefix_operator, TERMINAL_BITWISE_NOT);
    bit_array256_set(prefix_operator, TERMINAL_LOGICAL_NOT);
    bit_array256_set(prefix_operator, TERMINAL_STAR);
    bit_array256_set(prefix_operator, TERMINAL_AND);
    bit_array256_set(prefix_operator, SYMBOL_CAST);

    lr1_add_production_rule_set(prefix_operator, expression, not_higher_precedence_operator, SYMBOL_EXPRESSION);

    unsigned char partial_binary_operators[] = { SYMBOL_PARTIAL_MUL_DIV_MOD, SYMBOL_PARTIAL_ADD_SUB, 
        SYMBOL_PARTIAL_SHIFT, SYMBOL_PARTIAL_COMPARE, SYMBOL_PARTIAL_COMPARE_EQUAL, 
        SYMBOL_PARTIAL_BITWISE_AND, SYMBOL_PARTIAL_BITWISE_XOR, SYMBOL_PARTIAL_BITWISE_OR,
        SYMBOL_PARTIAL_LOGICAL_AND, SYMBOL_PARTIAL_LOGICAL_OR, SYMBOL_PARTIAL_ASSIGN, SYMBOL_PARTIAL_COMMA 
    };
#define N_PRECEDENCE_CLASSES sizeof(partial_binary_operators)
    // collection of disjoint sets of the operator in each group, replaced as the corresponding partial
    unsigned char precedence_classes[N_PRECEDENCE_CLASSES][4] = { 
        { TERMINAL_STAR, TERMINAL_DIV, TERMINAL_MOD }, { TERMINAL_PLUS, TERMINAL_MINUS }, { TERMINAL_LSHIFT, TERMINAL_RSHIFT }, 
        { TERMINAL_LESS, TERMINAL_LESS_EQUAL, TERMINAL_GREATER, TERMINAL_GREATER_EQUAL },
        { TERMINAL_EQUAL_EQUAL, TERMINAL_NOT_EQUAL }, { TERMINAL_AND }, { TERMINAL_BITWISE_XOR }, { TERMINAL_BITWISE_OR },
        { TERMINAL_LOGICAL_AND }, { TERMINAL_LOGICAL_OR }, { TERMINAL_ASSIGN }, { TERMINAL_COMMA }
    };
    unsigned char op;

    bit_array256_zero(partial_binary_operator);

    for (i = 0; i < N_PRECEDENCE_CLASSES; i++) {
        bit_array256_set(partial_binary_operator, partial_binary_operators[i]);
        lr1_add_production_rule_set(partial_binary_operator, expression, not_higher_precedence_operator, SYMBOL_EXPRESSION);
        bit_array256_unset(partial_binary_operator, partial_binary_operators[i]);
        bit_array256_zero(operator_set);
        for (j = 0; j < 4; j++) {
            op = precedence_classes[i][j];
            if (op == 0) break;
            bit_array256_set(operator_set, op);
        } 
        lr1_add_production_rule_set(expression, operator_set, all, partial_binary_operators[i]);
        bit_array256_not(operator_set);
        bit_array256_and(not_higher_precedence_operator, operator_set);
    }
}

void debug_grammar(void) {
    int i, j, k, l;
    unsigned long long lookahead_set[SC];
    unsigned char lookahead_bitstring[TC + 1];
    lookahead_bitstring[TC] = 0;
    for (i = 0; i < SC; i++) {
        for (j = 0; j < SC; j++) {
            for (k = 0; k < SC; k++) {
                lookahead_set[k] = 0ull;
            }
            for (k = 0; k < TC; k++) {
                lookahead_set[lr1_grammar[i][j][k]] |= 1ull << k;
            }
            for (k = 1; k < SC; k++) {
                if (lookahead_set[k]) {
                    for (l = 0; l < TC; l++) {
                        lookahead_bitstring[l] = ((lookahead_set[k] >> l) & 1) + '0';
                    }
                    
                    printf("%-12s %-12s { %s } -> %-12s\n", lr_tree_node_type_to_string(i), 
                        lr_tree_node_type_to_string(j), lookahead_bitstring, 
                        lr_tree_node_type_to_string(k));
                }
            }
        }
    }
}

static void string_to_terminals(char* str, unsigned char* terminals, unsigned long len) {
    unsigned long i;
    
    for (i = 0; i < len; i++) {
        switch (str[i]) {
        case ' ': case '\n': case '\t':
            terminals[i] = LR1_EPSILON;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
        case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
        case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case '_':
            terminals[i] = TERMINAL_LETTER;
            break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            terminals[i] = TERMINAL_NUMBER;
            break;
        case '+':
            if (str[i + 1] == '+') {
                terminals[i] = TERMINAL_INC;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_PLUS;
            }
            break;
        case '-':
            if (str[i + 1] == '-') {
                terminals[i] = TERMINAL_DEC;
                terminals[++i] = LR1_EPSILON;
            }
            else if (str[i + 1] == '>') {
                terminals[i] = TERMINAL_ARROW;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_MINUS;
            }
            break;
        case '.': terminals[i] = TERMINAL_DOT; break;
        case '(': terminals[i] = TERMINAL_OPEN_PAREN; break;
        case ')': terminals[i] = TERMINAL_CLOSE_PAREN; break;
        case '[': terminals[i] = TERMINAL_OPEN_BRACKET; break;
        case ']': terminals[i] = TERMINAL_CLOSE_BRACKET; break;
        case '{': terminals[i] = TERMINAL_OPEN_BRACE; break;
        case '}': terminals[i] = TERMINAL_CLOSE_BRACE; break;
        case '~': terminals[i] = TERMINAL_BITWISE_NOT; break;
        case '!': 
            if (str[i + 1] == '=') {
                terminals[i] = TERMINAL_NOT_EQUAL;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_LOGICAL_NOT;
            }
            break;
        case '*': terminals[i] = TERMINAL_STAR; break;
        case '/': 
            if (str[i + 1] == '/')
                for (; i < len && str[i] != '\n'; i++)
                    terminals[i] = LR1_EPSILON;
            else
                terminals[i] = TERMINAL_DIV; 
            break;
        case '%': terminals[i] = TERMINAL_MOD; break;
        case '<':
            if (str[i + 1] == '<') {
                terminals[i] = TERMINAL_LSHIFT;
                terminals[++i] = LR1_EPSILON;
            }
            else if (str[i + 1] == '=') {
                terminals[i] = TERMINAL_LESS_EQUAL;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_LESS;
            }
            break;
        case '>':
            if (str[i + 1] == '>') {
                terminals[i] = TERMINAL_RSHIFT;
                terminals[++i] = LR1_EPSILON;
            }
            else if (str[i + 1] == '=') {
                terminals[i] = TERMINAL_GREATER_EQUAL;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_GREATER;
            }
            break;
        case '=':
            if (str[i + 1] == '=') {
                terminals[i] = TERMINAL_EQUAL_EQUAL;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_ASSIGN;
            }
            break;
        case '&':
            if (str[i + 1] == '&') {
                terminals[i] = TERMINAL_LOGICAL_AND;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_AND;
            }
            break;
        case '^': terminals[i] = TERMINAL_BITWISE_XOR; break;
        case '|':
            if (str[i + 1] == '|') {
                terminals[i] = TERMINAL_LOGICAL_OR;
                terminals[++i] = LR1_EPSILON;
            }
            else {
                terminals[i] = TERMINAL_BITWISE_OR;
            }
            break;
        case ',': terminals[i] = TERMINAL_COMMA; break;
        case ';': terminals[i] = TERMINAL_END_STATEMENT; break;
        case '\'': terminals[i] = TERMINAL_SINGLE_QUOTE; break;
        case '"': terminals[i] = TERMINAL_QUOTE; break;
        default:
            terminals[i] = TERMINAL_NONE;
        }
    }
}
