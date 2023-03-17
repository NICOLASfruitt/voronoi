#if defined TYPE && defined EXT

#include <stdbool.h>

#define F_NAME_II(f_name, ext) f_name ## _ ## ext
#define F_NAME_I(f_name, ext) F_NAME_II(f_name, ext)
#define F_NAME(f_name) F_NAME_I(f_name, EXT)

#define VEC F_NAME(Vec)



typedef struct VEC VEC;
struct VEC {
  int next;
  int size;
  TYPE* arr;
};



VEC* F_NAME(vec_new)();
void F_NAME(vec_resize)(VEC* v);
void F_NAME(vec_push)(VEC* v, TYPE val);
bool F_NAME(vec_has)(VEC* v, TYPE val);

#endif
