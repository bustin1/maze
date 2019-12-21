#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


typedef unsigned int vertex;
typedef struct adjlist_header adjlist
struct adjlist_header
{
    vertex start;
    vertex vert;
    unsigned int weight;
    adjlist *next;
};

typedef struct graph_header graph;
struct graph_header
{
    unsigned int size;//number of vertices
    adjlist **adj;
};

bool is_acyclic(adjlist *L)
{
    if(L == NULL) return true;
    adjlist *tortoise = L;
    adjlist *hare = L->next;
    while(hare != NULL && hare->next != NULL)
    {
        tortoise = tortoise->next;
        hare = hare->next->next;
        if(hare == tortoise) return false;
    }
    return true;
}

bool is_vertex(graph *G, vertex v)
{
    return v < G->size;
}

bool is_in_adjlist(adjlist *L, vertex v)
{
    while(L != NULL)
    {
        if(L->vert == v) return true;
        L = L->next;
    }
    return false;
}

//checks valid adjlist
bool is_adjlist(graph *G, adjlist *L, vertex v)
{
    if(!is_acyclic(L)) return false;
    while(L != NULL)
    {
        if(!is_vertex(G, L->vert)) return false;
        if(!is_vertex(G, L->start)) return false;
        if(L->start != v) return false//start edge is always v
        if(L->vert == v) return false;//no self-edges
        if(!is_in_adjlist(G->adj[v], L->vert) return false//every edge goes into each node
        if(is_in_adjlist(L->next, v)) return false;//no dupes
        L = L->next;
    }
    return true;
}

//checks valid graph
bool is_graph(graph *G)
{
    if(G == NULL || G->adj == NULL) return false;
    for(vertex v=0; v<G->size; v++)
    {
        if(!is_adjlist(G, G->adj[v], v)) return false;
    }
    return true;
}

/*creates and returns a
* graph of size size */
graph* graph_new(int size)
{
    REQUIRES(size > 0);

    graph *G = malloc(sizeof(graph));
    G->size = size;
    G->adj = calloc(sizeof(adj *), size);

    ENSURES(is_graph(G));
    return G;
}
