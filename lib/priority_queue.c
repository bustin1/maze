#include "contracts.h"
#include "priority_queue.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef void *elem;
typedef bool has_higher_priority_fn(elem, elem); 

typedef struct priority_queue_header pqueue;
struct priority_queue_header
{
    elem *array;
    unsigned int size;
    int next;
    has_higher_priority_fn *fn;
};

bool ok_above(pqueue *Q, int child)
{
    int parent = child/2;
    return !(*Q->fn)(Q->array[child], Q->array[parent]);
}

//data structure invariant
bool is_safe(pqueue *Q)
{
    return Q != NULL && 
           Q->size > 0 && 
           Q->array != NULL &&
           Q->next > 0 && Q->next <= Q->size &&
           Q->fn != NULL;
}

//data structure invariant
bool is_ordered(pqueue *Q)
{
    int child = 2;
    while(child < Q->next)
    {
        if(!ok_above(Q,child)){
            printf("fail: %d\n", child);
            return false;
        }
        child++;
    }
    return true;
}


bool is_heap(pqueue *Q)
{
    return is_safe(Q) && is_ordered(Q);
}


pqueue *pq_new(int size, has_higher_priority_fn *fn)
{
    REQUIRES(size > 0);

    pqueue *Q = malloc(sizeof(pqueue));
    Q->array = calloc(size + 1, sizeof(void));
    Q->fn = fn;
    Q->next = 1;
    Q->size = size + 1;

    ENSURES(is_heap(Q));
    return Q;
}


bool pq_empty(pqueue *Q)
{
    REQUIRES(is_heap(Q));
    return Q->next == 1;
}

bool pq_full(pqueue *Q)
{
    REQUIRES(is_heap(Q));
    return Q->next == Q->size;
}

//a = parent, b = child
void swap_up(pqueue *Q, int a, int b)
{
    printf("swapping %p with %p", Q->array[b], Q->array[a]);
    elem tmp = Q->array[a];
    Q->array[a] = Q->array[b];
    Q->array[b] = tmp;
}

void pq_add(pqueue *Q, elem e)
{
    REQUIRES(is_heap(Q));
    REQUIRES(!pq_full(Q));

    Q->array[Q->next] = e;
    Q->next++;

    int i = Q->next - 1;
    while(i > 1)
    {
        if(!ok_above(Q, i)){
            swap_up(Q, i/2, i);
            i = i/2;
        }
        else{
            break;
        }
    }

    ENSURES(is_heap(Q));
    ENSURES(!pq_empty(Q));
}

elem pq_rem(pqueue *Q)
{
    REQUIRES(is_heap(Q));
    REQUIRES(!pq_empty(Q));

    Q->next--;
    elem A = Q->array[1];
    Q->array[1] = Q->array[Q->next];

    if(Q->next > 1){
        int i = 1;
        while(2*i < Q->next)
        {
            if(2*i + 1 < Q->next
               && (!ok_above(Q, 2*i) || !ok_above(Q, 2*i+1))){
                if((*Q->fn)(Q->array[2*i+1], Q->array[2*i])){
                    swap_up(Q, 2*i+1, i);
                    i = 2*i+1;
                }
                else{
                    swap_up(Q, 2*i, i);
                    i = 2*i;
                }
            }
            else if(!ok_above(Q, 2*i))
            {
                swap_up(Q, i, 2*i);
                i = 2*i;
            }
            else{
                return A;
            }
        }        
    }

    ENSURES(is_heap(Q));
    return A;
}

elem pq_peek(pqueue *Q)
{
    REQUIRES(is_heap(Q));
    return Q->array[1];
}

void pq_free(pqueue *Q)
{
    free(Q->array);
    free(Q);
}

unsigned int pq_size(pqueue *Q)
{
    ASSERT(is_heap(Q));
    return Q->next;
}

