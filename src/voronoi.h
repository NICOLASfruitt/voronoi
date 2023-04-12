#ifndef VORONOI_H
#define VORONOI_H

#include "structs.h"



typedef struct Voronoi Voronoi;
struct Voronoi {
  double y;
  PriorityQueue* pq; // Event queue
  Vec_ev* deleted;
  Vec_edge* edges;
  Arc* beachline;
};



Voronoi* v_new(Point sites[], int len);
void v_compute(Voronoi* v);
void display_beachline(Voronoi* v);

//Point par_intercept(Point a, Point b, double y);

#endif
