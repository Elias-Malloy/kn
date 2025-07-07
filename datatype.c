#include "datatype.h"
#include "grammar.h"
#include "lr_tree.h"
#include <stdbool.h>

typed_syntax_tree *typecheck(syntax_tree *tree) {
	unsigned char type, ltype, rtype, valid_typing, operator;
	syntax_tree *node, stmt;
	
	type = lr_tree_node_type(tree);
	if (type != SYMBOL_STATEMENTS) return NULL;
	
	node = tree;
	valid_typing = 1;
	do {
		stmt = node->right;
		if (stmt->left->type == SYMBOL_EXPRESSION) {
			valid_typing = typecheck_expression(stmt->left);
			if (!valid_typing) return NULL;
		}
	} while (node = node->left);
}

bool typecheck_expression(syntax_tree *expr) {
	unsigned char type, ltype, valid_typing;
	typed_syntax_tree *node, lhs, rhs;
	data_type etype;
	
	ltype = expr->left->type;
	// prepass introduce cast to equate to dominant type
	// mark dominant type/category
	// now each op as n not n2 cases
	// requires standard rules for type heirarchy which do exist
	switch (ltype) {
		case SYMBOL_EXPRESSION:
			// unary post
			// ugh ordering FUCK FUCK FUCK, when/where does the add go????
			break;
		// unary pre
		case TERMINAL_INC:
			
			node = lr_tree_node_create(EXPR_ADD, 
		case TERMINAL_DEC:
		
		case TERMINAL_
		case TERMINAL_PLUS:
		case TERMINAL_MINUS:
		case TERMINAL_BITWISE_NOT:
		case TERMINAL_LOGICAL_NOT:
		case TERMINAL_STAR:
			data_type = evaluate_expression_type(expr->right);
			if (data_type.indirection == 0) return 0;
			
		case TERMINAL_AND:
		
		// direct replacement (only action is to propegate type
		case SYMBOL_CONSTANT:
		case SYMBOL_VARIABLE:
		
		case SYMBOL_PARTIAL_MUL_DIV_MOD:
		case SYMBOL_PARTIAL_ADD_SUB:
		case SYMBOL_PARTIAL_SHIFT:
		case SYMBOL_PARTIAL_COMPARE:
		case SYMBOL_PARTIAL_COMPARE_EQUAL:
		case SYMBOL_PARTIAL_BITWISE_AND:
		case SYMBOL_PARTIAL_BITWISE_XOR:
		case SYMBOL_PARTIAL_BITWISE_OR:
		case SYMBOL_PARTIAL_LOGICAL_AND:
		case SYMBOL_PARTIAL_LOGICAL_OR:
		case SYMBOL_PARTIAL_ASSIGN:
		case SYMBOL_PARTIAL_COMMA:
			lhs = expr->left->left;
			operator = expr->left->right;
			rhs = expr->right;

			valid_typing = typecheck_expression(lhs);
			if (!valid_typing) return false;
			valid_typing = typecheck_expression(rhs);
			if (!valid_typing) return false;
		
			switch (operator) {
				case TERMINAL_STAR: // mul
					expr->type = EXPR_MUL;
					goto accept_integer_or_float;
				case TERMINAL_DIV:
					expr->type = EXPR_DIV;
					goto accept_integer_or_float;
				case TERMINAL_MOD;
					expr->type = EXPR_MOD;
					goto accept_integer;
				case TERMINAL_COMMA:
					expr->type = EXPR_SEQUENCE;
					expr->base_type = rhs->base_type;
					return true;
			}
			break;
	}
	accept_integer_or_float:
		if (lhs->base_type.prim_type == 
	accept_integer:
}

bool typecheck_statement(syntax_tree *stmt) {
	unsigned char ltype, rtype;
	
	ltype = stmt->left->type;
	if (ltype == SYMBOL_EXPRESSION) {
		
	}
}
