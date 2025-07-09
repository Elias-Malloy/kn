#pragma once

#include <stdint.h>

enum primitive_type {
	PRIM_VOID,
	PRIM_UINT8,
	PRIM_INT8,
	PRIM_UINT16,
	PRIM_INT16,
	PRIM_UINT32,
	PRIM_INT32,
	PRIM_UINT64,
	PRIM_INT64,
	PRIM_FLOAT32,
	PRIM_FLOAT64,
	PRIM_STRUCT,
};
/*
STMT_FOR
	initial     EXPR/DECL
    condition   EXPR
	increment   EXPR
	body		STMT_BLOCK
STMT_WHILE
	condition   EXPR
	body		STMT_BLOCK
STMT_IF
	condition	EXPR
	body		STMT_BLOCK
	else-body	STMT_BLOCK
STMT_RETURN
	value       EXPR

*/
enum typed_syntax_tree_node_type {
	PROGRAM,
	FUNCTION,
	CONSTANT,
	VARIABLE,
	DEFINITION,
	STATEMENT,
	STATEMENT_LIST,
	STATEMENT_PAIR,
	STMT_IF,
	STMT_FOR,
	STMT_WHILE,
	STMT_RETURN,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_LSHIFT,
	EXPR_RSHIFT,
	EXPR_CMP_LESS,
	EXPR_CMP_LESS_EQUAL,
	EXPR_CMP_GREATER,
	EXPR_CMP_GREATER_EQUAL,
	EXPR_CMP_EQUAL_EQUAL,
	EXPR_CMP_NOT_EQUAL,
	EXPR_BIT_AND,
	EXPR_BIT_XOR,
	EXPR_LOGICAL_AND,
	EXPR_LOGICAL_OR,
	EXPR_ASSIGN,
	EXPR_SEQUENCE,

}
/*
	EXPR_CAST_INT8_TO_INT64, // movsx
	EXPR_CAST_UINT8_TO_UINT64, // movzx
	EXPR_CAST_INT8_TO_INT32,
	EXPR_CAST_UINT8_TO_UINT32,
	EXPR_CAST_INT8_TO_INT16,
	EXPR_CAST_UINT8_TO_UINT16,
	EXPR_CAST_INT16_TO_INT64,
	EXPR_CAST_UINT16_TO_UINT64,
	EXPR_CAST_INT16_TO_INT32,
	EXPR_CAST_UINT16_TO_UINT32,
	EXPR_CAST_INT32_TO_INT64, // movsxd
	EXPR_CAST_UINT32_TO_UINT64,
	EXPR_CAST_FLOAT64_TO_FLOAT32, // cvtsd2ss
	EXPR_CAST_FLOAT32_TO_FLOAT64, // cvtsd2ss
	EXPR_CAST_FLOAT64_TO_INT64, // cvtsd2si
	EXPR_CAST_FLOAT64_TO_INT32, // cvtsd2si
	EXPR_CAST_FLOAT32_TO_INT64, // cvtss2si
	EXPR_CAST_FLOAT32_TO_INT32, // cvtss2si
	EXPR_CAST_INT64_TO_FLOAT64, // cvtsi2sd
	EXPR_CAST_INT32_TO_FLOAT64, // cvtsi2sd
	EXPR_CAST_INT64_TO_FLOAT32, // cvtsi2ss
	EXPR_CAST_INT32_TO_FLOAT32, // cvtsi2ss
*/

typedef struct typed_syntax_tree {
	uint16_t type;
	uint8_t primitive_type;
	uint8_t indirection;
	uint32_t _pad;
	struct typed_syntax_tree *left;
	struct typed_syntax_tree *right;
} typed_syntax_tree;

typedef struct syntax_tree {
	uint16_t type;
	uint16_t _pad0;
	uint32_t _pad;
	struct syntax_tree *left;
	struct syntax_tree *right;
} syntax_tree;

typedef struct syntax_tree_variable {
	uint16_t type;
	uint8_t primitive_type;
	uint8_t indirection;
	uint32_t _pad;
	char *name;
	struct {
		uint64_t field_count;
		struct syntax_tree_variable *fields;
	} *type_ext;
} syntax_tree_variable;

typedef struct base_data_type {
	unsigned char primitive_type;
	unsigned char indirection;
} base_data_type;

typedef struct complete_data_type {
	unint16_t pad0;
	base_data_type base_type;
	uint32_t field_count;
	char **field_names;
	struct complete_data_type *field_types;
} complete_data_type;

typed_syntax_tree *typecheck(syntax_tree *tree);
void syntax_tree_free(syntax_tree *tree);
void typed_syntax_tree_free(typed_syntax_tree *tree);