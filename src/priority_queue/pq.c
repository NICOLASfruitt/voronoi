// Peut-être à changer avec un tas ?!

#if defined V && defined P

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "pq.h"



struct Node {
  V v;
  P p;
  Node* next;
};

struct PriorityQueue {
  Node* head; 
};



PriorityQueue* pq_new() {
  PriorityQueue* pq = malloc(sizeof(PriorityQueue));
  pq->head = NULL;
  return pq;
}



void pq_add(PriorityQueue* pq, V v, P p) {
  Node* node = malloc(sizeof(Node));
  node->v = v;
  node->p = p;
  node->next = NULL;

  if (!pq->head || p <= pq->head->p) {
    node->next = pq->head;
    pq->head = node;
  }
  else {
    Node* c = pq->head;
    while (c->next && p > c->next->p)
      c = c->next;
    node->next = c->next;
    c->next = node;
  }
}



bool pq_is_empty(PriorityQueue* pq) {
  return pq->head == NULL;
}



V pq_peek(PriorityQueue* pq) {
  assert(!pq_is_empty(pq));
  return pq->head->v;
}



V pq_pop(PriorityQueue* pq) {
  assert(!pq_is_empty(pq));
  Node* h = pq->head;
  pq->head = pq->head->next;
  V v = h->v;
  free(h);
  return v;
}



void pq_display(PriorityQueue* pq, void v_display(V)) {
  if (pq_is_empty(pq))
    printf("Empty");
  else {
    printf("PQ [\n");
    Node* c = pq->head;
    while (c) {
      printf(" ");
      v_display(c->v);
      printf("\n");
      c = c->next;
    }
    printf("]");
  }
}

#endif
