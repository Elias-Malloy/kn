#pragma once

complete_syntax_tree *assign_locations_to_expressions(typed_syntax_tree *tree);

typedef struct graph {
	uint64_t vertex_count;
	uint8_t edge_weights[8];
} graph;

typedef struct live_in_out_sets {
	
} live_in_out_sets;

graph *create_graph(uint64_t vertex_count);
add_edge_to_graph(graph *g, uint64_t v1, uint64_t v2, uint8_t weight);
add_edge_to_undirected_graph(graph *g, uint64_t v1, uint64_t v2, uint8_t weight);

graph *create_interference_graph_from_tree(typed_syntax_tree *tree);