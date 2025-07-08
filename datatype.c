#include "datatype.h"
#include "grammar.h"
#include "lr_tree.h"
#include <stdbool.h>

typed_syntax_tree *typecheck(syntax_tree *tree) {
	unsigned char type, ltype, rtype, valid_typing, operator;
	syntax_tree *node, *stmt;
	
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

bool typecheck_expression(typed_syntax_tree *expr) {
	unsigned char type, ltype, valid_typing;
	typed_syntax_tree *node, *lhs, *rhs, *principal_node, *auxilary_node;
	complete_data_type *complete_type;
	
	ltype = expr->left->type;
	switch (ltype) {
		case SYMBOL_EXPRESSION:
			// unary post
			switch(expr->right->type) {
				// all the suffix ops
			}
			break;
		// unary pre
		case TERMINAL_INC:
		/*
			expr->type = EXPR_ADD;
			lhs = expr->right;
			rhs = lr_tree_node_create(EXPR_CONSTANT, NULL, 1);
			if (rhs == NULL) return false;
			rhs->base_type.prim_type = PRIM_UINT64;
			rhs->base_type.indirection = 0;
			node = expr->right;
			valid_typing = typecheck_expression(lhs);
			if (!valid_typing) return false;
			expr->base_type = node->base_type;
			expr->left = lhs;
			expr->right = rhs;
		*/	
			node = lr_tree_node_create(EXPR_ADD, 
		case TERMINAL_DEC:
		
		case TERMINAL_PLUS:
		case TERMINAL_MINUS:
		case TERMINAL_BITWISE_NOT:
			
		case TERMINAL_LOGICAL_NOT:
		case TERMINAL_STAR:
			
			
		case TERMINAL_AND:
		
		// direct replacement (only action is to propegate type
		case SYMBOL_CONSTANT:
		case SYMBOL_VARIABLE:
		
		// binary operators
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

			expr->left = lhs;
			expr->right = rhs;

			valid_typing = typecheck_expression(lhs);
			valid_typing &= typecheck_expression(rhs);
			
			if (lhs->base_type.indirection > rhs->base_type.indirection || (
				lhs->base_type.indirection == rhs->base_type.indirection &&
				lhs->base_type.prim_type >= rhs->base_type.prim_type)) {
				principal_node = lhs;
				auxilary_node = rhs;
			} else {
				principal_node = rhs;
				auxilary_node = lhs;
			}
			expr->base_type = principal_node->base_type;
			// reject if there was an type error on the left or right hand side
			if (!valid_typing) return false;

			// reject void/struct valued expressions
			if (auxilary_node->base_type.prim_type == PRIM_VOID ||
				principal_node->base_type.indirection == 0 && principal_node->base_type.prim_type == PRIM_STRUCT ||
				auxilary_node->base_type.indirection  == 0 && auxilary_node->base_type.prim_type  == PRIM_STRUCT)
				return false;

			switch (operator) {
				case TERMINAL_PLUS:
					expr->type = EXPR_ADD;
					goto reject_pointer_mixed_with_float;
				case TERMINAL_MINUS:
					expr->type = EXPR_SUB;
					goto accept_any;
    			case TERMINAL_STAR:
					expr->type = EXPR_MUL;
					goto accept_integer_or_float;
    			case TERMINAL_AND:
					expr->type = EXPR_BIT_AND;
					goto accept_integer;
    			case TERMINAL_DIV:
					expr->type = EXPR_DIV;
					goto accept_integer_or_float;
    			case TERMINAL_MOD:
					expr->type = EXPR_MOD;
					goto accept_integer;
    			case TERMINAL_LSHIFT:
					expr->type = EXPR_LSHIFT;
					goto accept_integer;
    			case TERMINAL_RSHIFT:
					expr->type = EXPR_RSHIFT;
					goto accept_integer;
    			case TERMINAL_LESS:
					expr->type = EXPR_CMP_LESS
					goto accept_any;
    			case TERMINAL_LESS_EQUAL:
					expr->type = EXPR_CMP_LESS_EQUAL;
					goto reject_pointer_mixed_with_float;
    			case TERMINAL_GREATER:
					expr->type = EXPR_CMP_GREATER;
					goto reject_pointer_mixed_with_float;
    			case TERMINAL_GREATER_EQUAL:
					expr->type = EXPR_CMP_GREATER_EQUAL;
					goto reject_pointer_mixed_with_float;
    			case TERMINAL_EQUAL_EQUAL:
					expr->type = EXPR_CMP_EQUAL_EQUAL;
					goto reject_pointer_mixed_with_float;
    			case TERMINAL_NOT_EQUAL:
					expr->type = EXPR_CMP_NOT_EQUAL;
					goto reject_pointer_mixed_with_float;
    			case TERMINAL_BITWISE_XOR:
					expr->type = EXPR_BIT_XOR;
					goto accept_integer;
    			case TERMINAL_BITWISE_OR:
					expr->type = EXPR_BIT_OR;
					goto accept_integer;
    			case TERMINAL_LOGICAL_AND:
					expr->type = EXPR_LOGICAL_AND;
					goto accept_any;
    			case TERMINAL_LOGICAL_OR:
					expr->type = EXPR_LOGICAL_OR;
					goto accept_any;
    			case TERMINAL_ASSIGN:
					// this one is complicated 
				case TERMINAL_COMMA:
					expr->type = EXPR_SEQUENCE;
					expr->base_type = rhs->base_type;
					return true;
			}
			break;
	}

accept_integer:
	if (principal_node->base_type.prim_type == PRIM_FLOAT32 || 
		principal_node->base_type.prim_type == PRIM_FLOAT64)
		return false;
accept_integer_or_float:
	if (principal_node->base_type.indirection > 0) return false;
	goto accept_any;
reject_pointer_mixed_with_float:
	if (principal_node->base_type.indirection > 0 && 
		auxilary_node->base_type.indirection == 0 && (
		auxilary_node->base_type.prim_type == PRIM_FLOAT32 || 
		auxilary_node->base_type.prim_type == PRIM_FLOAT64))
		return false;	
accept_any:	
	if (lhs->type == rhs->type) return true;
	
	complete_type = lr_tree_node_create(EXPR_TYPE, NULL, NULL);
	if (complete_type == NULL) return false;
	complete_type->base_type = principal_node->base_type;
	node = lr_tree_node_create(EXPR_CAST, complete_type, rhs);
	if (node == NULL) return false;
	if (auxilary_node->base_type.prim_type == lhs->base_type.prim_type)
		expr->left = node;
	else
		expr->right = node;
	
	return true;
}

bool typecheck_statement(syntax_tree *stmt) {
	unsigned char ltype, rtype;
	
	ltype = stmt->left->type;
	if (ltype == SYMBOL_EXPRESSION) {
		
	}
}
