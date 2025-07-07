#include "location.h"

typedef struct graph {
	uint64_t vertex_count;
	uint8_t edge_weights[8];
} graph;



typedef struct liveness_dataflow_sets {
	uint64_t vertex_count;
	bit_array **def;
	bit_array **in;
	bit_array **out;
} liveness_dataflow_sets;

graph *create_graph(uint64_t vertex_count) {
	graph *g = calloc(8 + vertex_count * vertex_count, 1);
	if (g == NULL) return NULL;
	g->vertex_count = vertex_count;
	return g;
}
void add_edge_to_graph(graph *g, uint64_t v1, uint64_t v2, uint8_t weight) {
	g->edge_weights[v1 * g->vertex_count + v2] = weight;
}
add_edge_to_undirected_graph(graph *g, uint64_t v1, uint64_t v2, uint8_t weight) {
	add_edge_to_graph(g, v1, v2, weight);
	add_edge_to_graph(g, v2, v1, weight);
}

graph *create_interference_graph_from_tree(typed_syntax_tree *tree) {
	// divide into blocks and mark all definitions and uses of variables
	// functions?
	
	//
}