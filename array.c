#include "array.h"

array64 array64_create(void) {
	array64 A;
	A.length = 0;
	A.data = malloc(16 * 8);
	if (A.data == NULL) return NULL;
	return A;
}

bool array64_append(array64 *A, uint64 v) {
	void *mem;
	
	if (!((A->length + 1) & 0xF)) {
		mem = realloc(A->data, (A->length + 17) * 8);
		if (mem == NULL) return false;
		A->data = mem;
	}
	A->data[A->length] = v; 
	A->length++;
	return true;
}