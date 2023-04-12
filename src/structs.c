#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"



#define TYPE T1
#define EXT  E1
#include "vector/vec.c"
#undef  TYPE
#undef  EXT

#define TYPE T2
#define EXT  E2
#include "vector/vec.c"
#undef  TYPE
#undef  EXT



Event* event_new(Point p) {
  Event* e = malloc(sizeof(Event));
  e->p = p;
  e->arc = NULL;
  e->type = Site;
  return e;
}



Point* point_from(double x, double y) {
  Point* p = malloc(sizeof(Point));
  p->x = x;
  p->y = y;
  return p;
}



Edge* edge_new() {
  Edge* e = malloc(sizeof(Edge));
  e->start = NULL;
  e->end = NULL;
  e->dir = NULL;
  e->twin = NULL;
  return e;
}



Edge* edge_from(double x1, double y1, double x2, double y2) {
  Point* a = point_from(x1, y1);
  Point* b = point_from(x2, y2);
  Edge* e = edge_new();
  e->start = a;
  e->end = b;
  return e;
}



Arc* arc_new(Point f) {
  Arc* arc = malloc(sizeof(Arc));
  arc->focus = f;
  arc->left = NULL;
  arc->right = NULL;
  arc->parent = NULL;
  arc->edge = edge_new();
  return arc;
}



void arc_set_left(Arc* arc, Arc* left) {
  arc->left = left;
  left->parent = arc;
}



void arc_set_right(Arc* arc, Arc* right) {
  arc->right = right;
  right->parent = arc;
}



Arc* arc_lmr_of(Arc* c) {
  // c leftmost right child of X
  while (c->parent && c->parent->left == c)
    c = c->parent;
  // Est NULL si c (initial) est le plus à gauche
  return c->parent;
}



Arc* arc_rml_of(Arc* c) {
  // c rightmost left child of X
  while (c->parent && c->parent->right == c)
    c = c->parent;
  // Est NULL si c (initial) est le plus à droite
  return c->parent;
}



Arc* arc_get_lmr(Arc* c) {
  // X leftmost right child of c
  c = c->right;
  while (c->left)
    c = c->left;
  return c;
}



Arc* arc_get_rml(Arc* c) {
  // X rightmost left child of c
  c = c->left;
  while (c->right)
    c = c->right;
  return c;
}