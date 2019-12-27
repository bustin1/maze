#include <stdio.h>
#include <math.h>
#include "lib/graph.h"

int largest_area_factor(int num, int root)
{
    for(int i=root; i>1; i--)
    {
        if(num%i == 0) return i;
    }
    return 1;
}


void printMaze(graph_t G)
{
    int root = (int)sqrt(graph_size(G));
    int width = largest_area_factor(graph_size(G), root);
    int length = graph_size(G) / width;

    for(int row=0; row<length; row++)
    {
        for(int col=0; col<width; col++)
        {
            printf("O");
            if(graph_hasedge(G, row*width+col, row*width+col + 1) && col != width - 1) printf("---");
            else printf("   ");
        }
        printf("\n");

        if(row != length - 1)
        {
            for(int col=0; col<width; col++)
            {
                if(graph_hasedge(G, row*width+col, (row+1)*(width)+col)) printf("|");
                else printf(" ");
                if(col != width-1) printf("   ");

            }
            printf("\n");
        }
    }

}

int main()
{
    graph_t G = prim(graph_dense_random_weight(25));
    printMaze(G);

    return 0;
}













