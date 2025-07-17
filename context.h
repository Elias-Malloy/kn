#pragma once

#include <stdint.h>
#include "array.h"

typedef struct variable {
	char *name;
	uint64_t definition;
	uint64_t *usages;
	uint64_t usage_count;
} variable;

typedef struct compile_context {
	array64 vars; 		        	// sorted by definition index
	array64 newlines;				// sorted indices of newlines;
	array64 scopes;					// sorted indices of open/close scopes, sign-bit=1 iff close scopes
} compile_context;

uint64_t start_of_current_scope(compile_context *ctx, uint64_t index);
uint64_t end_of_current_scope(compile_context *ctx, uint64_t index);
uint64_t lookup_variable_name(compile_context *ctx, char *name, uint64_t index);
bool is_subscope(compile_context *ctx, uint64_t inner, uint64_t outer);


