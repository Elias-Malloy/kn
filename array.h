#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct array64 {
	uint64_t length;
	uint64_t *data;
} array64;

array64 array64_create(void);
bool array64_append(array64 *A, uint64 v);