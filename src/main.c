#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "voronoi.h"



Point* gen_sites(int n);


int main() {
  //Point a = { 5.23, 9.69 };
  //Point b = { 6.41, 5.9 };
  //Point c = { 2.2, 3.4 };
  //Point sites[] = { a, b, c };
  
  int n = 5;
  Point* sites = gen_sites(n);
  Voronoi* v = v_new(sites, n);
  v_compute(v);
  
  printf("\nEDGES\n");
  for (int i = 0; i < v->closed_edges->next; i++) {
    Edge* e = v->closed_edges->arr[i];
    printf("(%f, %f), ", e->start->x, e->start->y);
    if (e->end)
      printf("(%f, %f)\n", e->end->x, e->end->y);
    else
      printf("NULL\n");
  }

  return 0;
}



Point* gen_sites(int n) {
  srand(101);
  Point* sites = malloc(n*sizeof(Point));
  for (int i = 0; i < n; i++) {
    double x = 100 * (double)rand()/RAND_MAX;
    double y = 100 * (double)rand()/RAND_MAX;
    Point p = { x, y };
    sites[i] = p;
  }
  return sites;
}
