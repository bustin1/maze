#include <stdlib.h>
#include <stdbool.h>

#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE

typedef struct adjacency_list_header adjlist;
typedef void *elem;
typedef struct priority_queue_header *pq_t;
typedef bool has_higher_priority(elem, elem);

pq_t pq_new(int size, has_higher_priority *fn);
elem pq_rem(pq_t Q);
elem pq_peek(pq_t Q);
void pq_add(pq_t Q, elem a);
bool pq_full(pq_t Q);
bool pq_empty(pq_t Q);
void pq_free(pq_t Q);
unsigned int pq_size(pq_t Q);

#endif
