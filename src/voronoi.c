#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "structs.h"
#include "voronoi.h"

#define LOGP(p) printf(#p "   (%6.2f, %6.2f)\n", p.x, p.y);



void v_add_arc(Voronoi* v, Point p);
void v_remove_arc(Voronoi* v, Point point, Arc* par);
void v_circle_events(Voronoi* v,  Arc* arc);
Arc* get_arc(Voronoi* v, double x);
double eval_par(Point f, double y, double x);
Point par_intercept(Point a, Point b, double y);
bool line_intercept(Point* i, Point a, Point u, Point b, Point v);
double dist(Point a, Point b);



Voronoi* v_new(Point* sites, int len) {
  Voronoi* v = malloc(sizeof(Voronoi));
  v->y = 0;
  v->pq = pq_new();
  v->deleted = vec_new_ev();
  v->edges = vec_new_edge();
  v->closed_edges = vec_new_edge();
  v->beachline = NULL;

  for (int i = 0; i < len; i++) {
    Event* e = event_new(sites[i]);
    pq_add(v->pq, e, e->p.y);
  }
  return v;
}



void v_compute(Voronoi* v) {
  while (!pq_is_empty(v->pq)) {
    //display_beachline(v);

    Event* e = pq_pop(v->pq);
    v->y = e->p.y;
    if (e->type == Site) {
      //printf("Site (%6.2f, %6.2f)\n", e->p.x, e->p.y);
      v_add_arc(v, e->p);
    }
    else if (!vec_has_ev(v->deleted, e)){ // jsp si définitif
      //printf("Circle (%6.2f, %6.2f)\n", e->p.x, e->p.y);
      v_remove_arc(v, e->p, e->arc);
    }
  }
  //display_beachline(v);
}



void v_add_arc(Voronoi* v, Point p) {
  if (v->beachline == NULL) {
    v->beachline = arc_new(p);
    return;
  }

  Arc* arc = get_arc(v, p.x);
  if (arc->event) {
    vec_push_ev(v->deleted, arc->event);
    arc->event = NULL;
  }

  // sépare la beachline en 3 nouvelles paraboles
  Arc* a = arc_new(arc->focus);
  Arc* b = arc_new(p);
  Arc* c = arc_new(arc->focus);

  arc_set_left(arc, arc_new(p));
  arc_set_left(arc->left, a);
  arc_set_right(arc->left, b);
  arc_set_right(arc, c);

  // nouveaux segments
  Point* i = point_from(p.x, eval_par(arc->focus, v->y, p.x));
  arc->edge->start = i;
  arc->left->edge->start = i;
  vec_push_edge(v->edges, arc->edge);
  vec_push_edge(v->edges, arc->left->edge);

  v_circle_events(v, a);
  v_circle_events(v, b);
  v_circle_events(v, c);
}



void v_remove_arc(Voronoi* v, Point point, Arc* a) {
  assert(v->beachline != NULL);

  Arc* p_left = arc_lmr_of(a);
  Arc* p_right = arc_rml_of(a);
  
  if (!p_left || !p_right)  // inutile (devrait ne jamais arriver)
    return;

  Arc* left = arc_get_rml(p_left);    // à gauche de a
  Arc* right = arc_get_lmr(p_right);  // à droite de a

  if (left->event) {
    vec_push_ev(v->deleted, left->event);
    left->event = NULL;
  }
  if (right->event) {
    vec_push_ev(v->deleted, right->event);
    right->event = NULL;
  }

  Arc* gp = a->parent->parent;
  // point à égale distance de a, left et right
  Point* i = point_from(point.x, eval_par(a->focus, v->y, point.x));

  // clos les segments
  Edge* el = p_left->edge;
  Edge* er = p_right->edge;
  el->end = i;
  er->end = i;
  vec_push_edge(v->closed_edges, el);
  vec_push_edge(v->closed_edges, er);
 
  // nouveau segment
  Edge* e = edge_new();
  e->start = i;
  Arc* higher;
  Arc* c = a;
  while (c->parent) {
    if (c == p_left)
      higher = p_left;
    if (c == p_right)
      higher = p_right;
    c = c->parent;
  }
  higher->edge = e;
  vec_push_edge(v->edges, e);

  // résout les nouveaux liens de parenté
  if (a == a->parent->left) {
    if (a->parent == gp->left)
      arc_set_left(gp, a->parent->right);
    else
      arc_set_right(gp, a->parent->right);
  }
  else {
    if (a->parent == gp->left)
      arc_set_left(gp, a->parent->left);
    else
      arc_set_right(gp, a->parent->left);
  }

  free(a->parent);
  free(a);

  v_circle_events(v, left);
  v_circle_events(v, right);
}



void v_circle_events(Voronoi* v, Arc* a) {
  Arc* p_left = arc_lmr_of(a);
  Arc* p_right = arc_rml_of(a);

  if (!p_left || !p_right)  // arc le plus à gauche/droite
    return;

  Arc* left = arc_get_rml(p_left);    // à gauche de a
  Arc* right = arc_get_lmr(p_right);  // à droite de a
  
  // ??? à expliquer
  if (left->focus.x == right->focus.x && left->focus.y == right->focus.y)
    return;

  Point pl = *p_left->edge->start;
  Point ml = { (a->focus.x + left->focus.x)/2., (a->focus.y + left->focus.y)/2. };
  Point u = { ml.x - pl.x, ml.y - pl.y };

  Point pr = *p_right->edge->start;
  Point mr = { (a->focus.x + right->focus.x)/2., (a->focus.y + right->focus.y)/2. };
  Point w = { mr.x - pr.x, mr.y - pr.y };

  // point, s'il existe, à égale distance de a, left et right
  Point i;
  if (!line_intercept(&i, pl, u, pr, w))
    return;
  i.y += dist(i, a->focus);
  if (i.y < v->y)
    return;

  // event pour supprimer l'arc
  Event* e = event_new(i);
  e->arc = a;
  e->type = Circle;
  a->event = e;
  pq_add(v->pq, e, e->p.x);
}



Arc* get_arc(Voronoi* v, double x) {
  Arc* c = v->beachline;
  while (c && c->left && c->right) { // c->left ssi c->right normalement
    Point i = par_intercept(c->left->focus, c->right->focus, v->y);
    if (x < i.x)
      c = c->left;
    else
      c = c->right;
  }
  return c;
}



double eval_par(Point f, double y, double x) {
  // points à égale distance entre F et la droite y (parabole)
  return (x - f.x)*(x - f.x)/(2.*(f.y - y)) + (f.y + y)/2.;
}



Point par_intercept(Point a, Point b, double y) {
  // pacine de Ax² + Bx + C pertinente
  // pour get_par dans i
  double A = (b.y - a.y)/(2.*(a.y - y)*(b.y - y));
  double B = (b.x*(a.y - y) - a.x*(b.y - y))/((a.y - y)*(b.y - y));
  double C = a.x*a.x/(2.*(a.y - y)) - b.x*b.x/(2.*(b.y - y)) + (a.y - b.y)/2.;

  double delta = B*B - 4.*A*C;
  // Pas de solution si a = b
  assert(delta >= 0.);
  double x1 = (-B - sqrtf(delta))/(2.*A);
  double x2 = (-B + sqrtf(delta))/(2.*A);

  Point p;
  if (fabs(y - a.y) < fabs(y - b.y))
    p.x = fmax(x1, x2);
  else
    p.x = fmin(x1, x2);
  p.y = eval_par(a, y, p.x);
  return p;
}



bool line_intercept(Point* i, Point a, Point u, Point b, Point v) {
  // résout A + pU = B + qV
  // renvoie true si i est modifié
  double d = u.x*v.y - u.y*v.x;
  // droites parallèles
  if (d == 0.)
    return false;
  double p = (v.y*(b.x - a.x) + v.x*(a.y - b.y))/d;

  i->x = a.x + p*u.x;
  i->y = a.y + p*u.y;
  return true;
}



double dist(Point a, Point b) {
  return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}



void display_rec(Arc* c, int pad, const char* prefix) {
  assert(c);  // arbre non entier (ne peut pas arriver)
  printf("%s(%6.2f, %6.2f)", prefix, c->focus.x, c->focus.y);
  if (c->right || c->left) {
    display_rec(c->right, pad + 16 + 3, "┬──");
    for (int i = 0; i < pad; i++)
      putchar(' ');
    display_rec(c->left, pad + 16 + 3, "└──");
  }
  else
    printf("\n");
}



void display_beachline(Voronoi* v) {
  printf("Beachline :\n");
  if (v->beachline == NULL)
    printf(" Empty\n");
  else
    display_rec(v->beachline, 16, "");
}
