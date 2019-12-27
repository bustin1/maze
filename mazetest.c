#include "lib/graph.h"
#include <stdio.h>
#include <time.h>

int main()
{

    graph_t G1 = graph_new(6);
    graph_addedge(G1, 0, 1, 5);
    graph_addedge(G1, 1, 2, 3);
    graph_addedge(G1, 2, 3, 5);
    graph_addedge(G1, 3, 0, 3);
    graph_addedge(G1, 3, 4, 4);
    graph_addedge(G1, 2, 4, 6);
    graph_addedge(G1, 0, 4, 7);
    graph_addedge(G1, 0, 5, 10);
    graph_print(G1);

    graph_t G2 = prim(G1);
    graph_print(G2);
    graph_free(G1);
    graph_free(G2);

    printf("\n");

    printf("testing completed. Now complete the graph\n");

    return 0;
}













