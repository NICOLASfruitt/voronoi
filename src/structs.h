#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>

typedef struct Point Point;
typedef struct Edge Edge;
typedef struct Arc Arc;
typedef struct Event Event;

#define T1 Event*
#define E1 ev
#define T2 Edge*
#define E2 edge

#define TYPE T1
#define EXT  E1
#include "vector/vec.h"
#undef  TYPE
#undef  EXT

#define TYPE T2
#define EXT  E2
#include "vector/vec.h"
#undef  TYPE
#undef  EXT

#define V Event*
#define P int
#include "priority_queue/pq.h"
#undef  V
#undef  P

#define LOGP(p) printf(#p "   (%6.2f, %6.2f)\n", p.x, p.y);

struct Point {
  double x;
  double y;
};

struct Edge {
  Point* start;
  Point* end;
  Point* dir;
  Edge* twin;
};

struct Arc {
  Point focus;
  Arc* left;
  Arc* right;
  Arc* parent;
  Edge* edge;
  Event* event;
};

enum EventType {
  Site,
  Circle,
};

struct Event {
  Point p;
  Arc* arc;
  enum EventType type;
};

Point* point_from(double x, double y);
Edge* edge_new();
Edge* edge_from(double x1, double y1, double x2, double y2);
Event* event_new(Point p);
Arc* arc_new(Point f);
void arc_set_left(Arc* arc, Arc* left);
void arc_set_right(Arc* arc, Arc* right);
Arc* arc_lmr_of(Arc* c);
Arc* arc_rml_of(Arc* c);
Arc* arc_get_lmr(Arc* c);
Arc* arc_get_rml(Arc* c);

#endif