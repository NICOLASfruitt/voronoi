#if defined V && defined P

#include <stdbool.h>



typedef struct Node Node;
typedef struct PriorityQueue PriorityQueue;



PriorityQueue* pq_new();
void pq_add(PriorityQueue* pq, V v, P p);
bool pq_is_empty(PriorityQueue* pq);
V pq_peek(PriorityQueue* pq);
V pq_pop(PriorityQueue* pq);
void pq_display(PriorityQueue* pq, void v_display(V));

#endif
