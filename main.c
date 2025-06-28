#include "parse.h"
#include "lr_tree.h"

#define N_BENCHMARK_RUNS 100000

int main(void) {
	char* input_string;
	unsigned long long i;
	lr_tree_node* parse_tree;

	write_grammar();
	debug_grammar();
	
	input_string = "7 + 54 * ++241++; 1 || ~120 + 44;";
	parse_tree = parse_string(input_string);

	return 0;
}
