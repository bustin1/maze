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

void printTree(graph_t G, int width, int length)
{
    for(int row=0; row<length; row++)
    {
        for(int col=0; col<width; col++)
        {
            printf("O");
            if(graph_hasedge(G, row*width+col, row*width+col+1) && col != width - 1)
                printf("---");
            else
                printf("   ");
        }
        printf("\n");
        if(row != length-1){
            for(int col=0; col<width; col++)
            {
                if(graph_hasedge(G, row*width+col, (row+1)*width+col))
                    printf("|");
                else
                    printf(" ");
                if(col != width-1) printf("   ");
            }
            printf("\n");
       }
    }
}

//width of spanning tree;
//length of spanning tree;
//width+1 and length+1 is the 
//dim of the maze
void printMaze(graph_t G, int width, int length)
{
    for(int row=0; row<length+1; row++)
    {
        for(int col=0; col<width+1; col++)
        {
            printf("O");
            if(((row == 0 || row == length) ||
                !graph_hasedge(G, (row-1)*width+col, row*width+col)) 
                && col != width){
                printf("---");
            }
            else printf("   ");
        }
        printf("\n");
        if(row != length)
        {
            for(int col=0; col<width+1; col++)
            {
                if(!(row == 0 && col == 0) && !(row == length-1 && col == width) &&
                    ((!graph_hasedge(G, row*width+col-1, row*width+col)
                   || (col == 0 || col == width) ))){
                    printf("|");
                }
                else printf(" ");
                if(col != width) printf("   ");

            }
        }
        printf("\n");
    }

}

int main()
{
    //printf("here\n");
    int num_of_vert = 121;
    int root = (int)sqrt(num_of_vert);
    int length = largest_area_factor(num_of_vert, root);
    int width = num_of_vert / length;
    graph_t G2 = graph_dense_random_weight(num_of_vert, width, length);
    graph_t G = prim(G2);
    printTree(G, width, length);
    printf("\n\n");
    printMaze(G, width, length);

    graph_free(G2);
    graph_free(G);

    return 0;
}













