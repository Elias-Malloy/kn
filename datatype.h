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

enum typed_tree_node_type {
	EXPR_ASSIGN,
	EXPR_ADD_INT64,
	EXPR_ADD_INT32,
	EXPR_ADD_INT16,
	EXPR_ADD_INT8,
	EXPR_ADD_FLOAT64,
	EXPR_ADD_FLOAT32,
	EXPR_SUB_INT64,
	EXPR_SUB_INT32,
	EXPR_SUB_INT16,
	EXPR_SUB_INT8,
	EXPR_SUB_FLOAT64,
	EXPR_SUB_FLOAT32,
	EXPR_MUL_INT64,   // imul
	EXPR_MUL_UINT64,  // mul
	EXPR_MUL_INT32,
	EXPR_MUL_UINT32,
	EXPR_MUL_INT16,
	EXPR_MUL_UINT16,
	EXPR_MUL_INT8,
	EXPR_MUL_UINT8,
	EXPR_MUL_FLOAT64, // mulsd
	EXPR_MUL_FLOAT32, // mulss
	EXPR_DIV_INT64,
	EXPR_DIV_UINT64,
	EXPR_DIV_INT32,
	EXPR_DIV_UINT32,
	EXPR_DIV_INT16,
	EXPR_DIV_UINT16,
	EXPR_DIV_INT8,
	EXPR_DIV_UINT8,
	EXPR_DIV_FLOAT64, // divsd xmm, xmm/m64 (lhs must be reg)
	EXPR_DIV_FLOAT32, // divss
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
};

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

typedef struct data_type {
	unsigned char base_type;
	unsigned char indirection;
} data_type;

typed_syntax_tree *typecheck(syntax_tree *tree);