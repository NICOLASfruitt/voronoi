#if defined TYPE && defined EXT

#include <stdlib.h>
#include <stdbool.h>




VEC* F_NAME(vec_new)() {
  VEC* v = malloc(sizeof(VEC));
  v->next = 0;
  v->size = 16;
  v->arr = malloc(v->size*sizeof(TYPE));
  return v;
}



void F_NAME(vec_resize)(VEC* v) {
  int size = 2*v->size;
  TYPE* arr = malloc(size*sizeof(TYPE));
  for (int i = 0; i < v->size; i++)
    arr[i] = v->arr[i];
  free(v->arr);
  v->size = size;
  v->arr = arr;
}



void F_NAME(vec_push)(VEC* v, TYPE val) {
  if (v->next == v->size)
    F_NAME(vec_resize)(v);
  v->arr[v->next] = val;
  v->next++;
}



bool F_NAME(vec_has)(VEC* v, TYPE val) {
  for (int i = 0; i < v->next; i++)
    if (v->arr[i] == val)
      return true;
  return false;
}

#endif
