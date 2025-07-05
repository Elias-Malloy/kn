#include "lr_tree.h"
#include "grammar.h"
#include "codefmt.h"

void compile_statement(Program *program, lr_tree_node *stmt);
void compile_expression(Program *program, lr_tree_node *expr);

int generate_code(Program *program, lr_tree_node *tree) {
	unsigned char type;
	lr_tree_node *node;
	
	node = tree;
	type = lr_tree_node_type(node);
	
	while (type == SYMBOL_STATEMENTS) {
		compile_statement(program, node->right);
		node = node->left;
		type = lr_tree_node_type(node);
	}
	
}


void compile_statement(Program *program, lr_tree_node *stmt) {
	unsigned char type;
	
	type = lr_tree_node_type(stmt->left);
	
	if (type == SYMBOL_EXPRESSION) {
		compile_expression(stmt->left);
	}
}

void compile_expression(Program *program, lr_tree_node *expr) {
	unsigned char type;
	Argument args[4];
	type = lr_tree_node_type(expr->left);
	
	switch(type) {
		case TERMINAL_INC:
			add_instruction(program, INSTR_ADD, );
		case TERMINAL_DEC:
		
		case SYMBOL_PARTIAL_DIRECT_ACCESS:
		case SYMBOL_PARTIAL_INDIRECT_ACCESS:
		case SYMBOL_PARTIAL_FUNCTION_CALL:
		case SYMBOL_PARTIAL_SUBSCRIPT:
		case SYMBOL_PARTIAL_CAST:
		case SYMBOL_CAST:
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
	}
}