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

bool graph_hasedge(graph *G, vertex v, vertex w)
{
    REQUIRES(is_graph(G));
    return is_in_adjlist(G->adj[v], w);
}

void graph_addedge(graph *G, vertex v, vertex w, int weight)
{
    REQUIRES(is_vertex(G, v) && is_vertex(G, w));
    REQUIRES(is_graph(G));

    adjlist *L1 = malloc(sizeof(adjlist));
    L1->start = v;
    L1->vert = w;
    L1->weight = weight;
    L1->next = G->adj[v];
    G->adj[v] = L1;

    adjlist *L2 = malloc(sizeof(adjlist));
    L2->start = w;
    L2->vert = v;
    L2->weight = weight;
    L2->next = G->adj[w];
    G->adj[w] = L2;

    ENSURES(is_graph(G));
    ENSURES(graph_hasedge(G, v, w));
    ENSURES(graph_hasedge(G, w, v));
}

void graph_free(graph *G)
{
    REQUIRES(is_graph(G));
    for(vertex v=0; v<G->size; v++)
    {
        adjlist *L1 = G->adj[v];
        adjlist *L2 = G->adj[v];
        while(L1 != NULL)
        {
            L2 = L1->next;
            free(L1);
            L1 = L2;
        }
    }
    free(G->adj);
    free(G);
}

void graph_print(graph *G)
{
    REQUIRES(is_graph(G));
    for(vertex v=0; v<G->size; v++)
    {
        printf("%d: ",v);
        adjlist *L = G->adj[v];
        while(L != NULL)
        {
            if(L->next == NULL) printf("%d\n", L->vert);
            else printf("%d, ", L->vert);
        }
    }
    ENSURES(is_graph(G));
}

unsigned int graph_size(graph *G)
{
    REQUIRES(is_graph(G));
    return G->size;
}

typedef struct neighbor_header neighbor;
struct neighbor_header
{
    adjlist *nbors;
}

neighbor* graph_get_neighbors(graph *G, vertex)
{
    REQUIRES(is_graph(G));
    neighbor *N = malloc(sizeof(neighbor));
    N->nbors = G->adj[v];
    return N;
}

bool graph_hasmore_neighbors(neighbor *N)
{
    return N != NULL;
}

adjlist* graph_next_neighbor(neighbor *N)
{
    REQUIRES(graph_hasmore_neighbors(N));
    adjlist *L = N->nbors;
    N->nbors = N->nbors->next;
    return L;
}

void graph_free_neighbors(neighbor *N)
{
    REQUIRES(N != NULL);
    free(N);
}










