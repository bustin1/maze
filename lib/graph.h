#include <stdbool.h>

#ifndef _GRAPH_H
#define _GRAPH_H

typedef struct graph_header *graph_t;
typedef unsigned int vertex;

graph_t graph_new(int size);                                //O(1)
bool graph_hasedge(graph_t G, vertex v, vertex w);          //O(min(v,e))
void graph_addedge(graph_t G, vertex v, vertex w);          //O(1)
//void graph_addedge_weight(graph_t G, vertex v, vertex w, unsigned int weight);          //O(1)
void graph_free(graph_t G);                                 //O(e+v)
//graph_t graph_dense(int size);                              //O(v*v)
void graph_print(graph_t G);                                //O(e+v)
unsigned int graph_size(graph_t G);                         //O(1)

typedef struct neighbor_header *neighbor_t;         
bool graph_hasmore_neighbors(neighbor_t N);                 //O(1)
neighbor_t graph_get_neighbors(graph_t G, vertex v);        //O(1)
//vertex graph_next_neighbor(neighbor_t N);                   //O(1)
void graph_free_neighbors(neighbor_t N);                    //O(1)

bool dfs_search(graph_t G, vertex start, vertex target);    //O(v+e)

graph_t vertex_centric(graph_t G);                          //O(e)
graph_t prim(graph_t G);                          //O(e)

#endif
