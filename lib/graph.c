#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "graph.h"
#include "contracts.h"
#include "priority_queue.h"


typedef unsigned int vertex;
typedef struct adjacency_list_header adjlist;
struct adjacency_list_header
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
        if(L->start != v) return false;//start edge is always v
        if(L->vert == v) return false;//no self-edges
        if(!is_in_adjlist(G->adj[v], L->vert)) return false;//every edge goes into each node
        if(is_in_adjlist(L->next, L->vert)) return false;//no dupes
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
    G->adj = calloc(sizeof(adjlist *), size);

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

graph* graph_dense_random_weight(int size, int width, int length)
{
    REQUIRES(size > 0);

    graph *G = graph_new(size);
    srand(time(0));
    for(vertex row=0; row<length; row++)
    {
        for(vertex col=0; col<width-1; col++)
        {
            graph_addedge(G, row*width+col, row*width+col+1, rand());
            if(row != length-1) graph_addedge(G, row*width+col, (row+1)*width+col, rand());
        }
    }
    //graph_addedge(G, width*length-2, width*length-1, rand());
    graph_addedge(G, width*(length-1)-1, width*length-1, rand());
    
    ENSURES(is_graph(G));
    return G;
    
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
            L = L->next;
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
};

neighbor* graph_get_neighbors(graph *G, vertex v)
{
    REQUIRES(is_graph(G));

    neighbor *N = malloc(sizeof(neighbor));
    N->nbors = G->adj[v];

    ENSURES(is_graph(G));
    return N;
}

bool graph_hasmore_neighbors(neighbor *N)
{
    REQUIRES(N != NULL);
    //if(N->nbors != NULL) printf("%p ", N->nbors);
    return N->nbors != NULL;
}

adjlist* graph_next_neighbor(neighbor *N)
{
    REQUIRES(graph_hasmore_neighbors(N));
    adjlist *L = N->nbors;
    N->nbors = N->nbors->next;
    //if(N->nbors == NULL) printf("%p ", N->nbors);
    return L;
}

void graph_free_neighbors(neighbor *N)
{
    REQUIRES(N != NULL);
    free(N);
}

bool dfs_search_helper(graph *G, vertex start, vertex target, bool *visited)
{
    if(start == target) return true;
    visited[start] = true;

    neighbor *N = graph_get_neighbors(G, start);
    while(graph_hasmore_neighbors(N))
    {
        adjlist *L = graph_next_neighbor(N);
        if(!visited[L->vert] && dfs_search_helper(G, L->vert, target, visited))
        {
            return true;
        }
    }
    graph_free_neighbors(N);
    return false;
}

bool dfs_search(graph *G, vertex start, vertex target)
{
    REQUIRES(is_vertex(G, start) && is_vertex(G, target));
    REQUIRES(is_graph(G));

    bool *visited = calloc(sizeof(bool), G->size);
    bool found = dfs_search_helper(G, start, target, visited);
    free(visited);

    ENSURES(is_graph(G));
    return found;
}

void vertex_centric_helper(graph *G, graph *G2, vertex start, bool *visited)
{
    visited[start] = true;

    neighbor *N = graph_get_neighbors(G, start);
    while(graph_hasmore_neighbors(N))
    {
        adjlist *L = graph_next_neighbor(N);
        if(!visited[L->vert])
        {
            graph_addedge(G2, start, L->vert, L->weight);
            vertex_centric_helper(G, G2, L->vert, visited);
        }
    }
    graph_free_neighbors(N);
}
            

graph* vertex_centric(graph *G)
{
    REQUIRES(is_graph(G));

    bool *visited = calloc(sizeof(bool), G->size);
    graph *G2 = graph_new(G->size);
    
    int counter = 0;
    vertex start = 0;
    while(counter < G2->size)
    {
        vertex_centric_helper(G, G2, start, visited);
        for(vertex v=0; v<G2->size; v++)
        {
            if(visited[v]) counter++;
            else{
                start = v;
                break;
            }
        }
    }
    free(visited);

    ENSURES(is_graph(G));
    ENSURES(is_graph(G2));
    return G2;
}

bool higher_prior(elem e1, elem e2)
{
    REQUIRES(e1 != NULL && e2 != NULL);
    return ((adjlist*)e1)->weight < ((adjlist*)e2)->weight;
}

void prim_helper(graph *G, graph *G2, vertex start, bool *visited)
{
    visited[start] = true;

    pq_t Q = pq_new(G->size*(G->size), &higher_prior);

    neighbor *N = graph_get_neighbors(G, start);
    while(graph_hasmore_neighbors(N))
    {
        adjlist *L = graph_next_neighbor(N);
        if(!visited[L->vert])
        {
            adjlist *L2 = N->nbors;
            pq_add(Q, (elem)L);
        }
    }
    graph_free_neighbors(N);

    while(!pq_empty(Q))
    {
        adjlist *L = (adjlist *)pq_rem(Q);
        if(!visited[L->vert])
        {
            graph_addedge(G2, L->start, L->vert, L->weight);
            visited[L->vert] = true;
        }

        N = graph_get_neighbors(G, L->vert);
        while(graph_hasmore_neighbors(N))
        {
            adjlist *L2 = graph_next_neighbor(N);
            if(!visited[L2->vert])
            {
                pq_add(Q, (elem)L2);
            }
        }
        graph_free_neighbors(N);
    }
    pq_free(Q);

}
            

graph* prim(graph *G)
{
    REQUIRES(is_graph(G));

    bool *visited = calloc(sizeof(bool), G->size);
    graph *G2 = graph_new(G->size);
    
    int counter = 0;
    vertex start = 0;
    while(counter < G2->size)
    {
        prim_helper(G, G2, start, visited);
        for(vertex v=0; v<G2->size; v++)
        {
            if(visited[v]) counter++;
            else{
                start = v;
                break;
            }
        }
    }
    free(visited);

    ENSURES(is_graph(G));
    ENSURES(is_graph(G2));
    return G2;
}

















