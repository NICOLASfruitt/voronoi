#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "voronoi.h"
#include "shaders.h"

#define SIZE 800

double* test(Point* sites, int n, int* e_n);
Point* gen_sites(int n);
double* concat(double* a, double* b, int n, int m);



int main() {
  glfwInit();

  GLFWwindow* window = glfwCreateWindow(SIZE, SIZE, "Voronoi", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, SIZE, SIZE);

  Point a = { 5.23, 9.69 };
  Point b = { 6.41, 5.9 };
  Point c = { 2.2, 3.4 };
  Point sites[] = { a, b, c };
  int n = 3;

  //int n = 10;
  //Point* sites = gen_sites(n);
  double* points = malloc(2*n*sizeof(double));
  for (int i = 0; i < n; i++) {
    points[2*i] = sites[i].x;
    points[2*i+1] = sites[i].y;
    //printf("(%f, %f)\n", sites[i].x, sites[i].y);
  }

  int m;
  double* edges = test(sites, n, &m);

  double* vertices = concat(points, edges, 2*n, 2*m);
  
  float mat[] = {
    .02, 0., -1.,
    0., .02, -1.,
    0., 0., 0.,
  };

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (n+m)*2*sizeof(double), vertices, GL_STATIC_DRAW);

  GLuint program = createBasicProgram(vertex_source, fragment_source);

  glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), (void*)0);
  glEnableVertexAttribArray(0);

  GLint mLoc = glGetUniformLocation(program, "m");
  GLint colorLoc = glGetUniformLocation(program, "color");
  
  glPointSize(4.);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glUniformMatrix3fv(mLoc, 1, GL_TRUE, mat);
    glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, n);

    glUniform4f(colorLoc, 0.0, 1.0, 0.2, 1.0);
    glDrawArrays(GL_LINES, n, m);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}



void disp_edge(Edge* e) {
  assert(e);

  if (e->start)
    printf("(%f, %f), ", e->start->x, e->start->y);
  else
    printf("NULL, ");
  printf("(%f, %f), ", e->dir->x, e->dir->y);
  if (e->end)
    printf("(%f, %f)\n", e->end->x, e->end->y);
  else
    printf("NULL\n");
}

void _close(Point* p, Point* dir, Point* t) {
  assert(dir->x != 0. && dir->y != 0.);

  if (dir->x != 0.) {
  }
  else {

  }
}

Edge** close_edges(Edge** edges, int n) {
  int m = n;
  for (int i = 0; i < n; i++) {
    Edge* e = edges[i];

    if (e->twin) {
      m += 1;
      if (!e->end) {

      }
      else {

      }
    }
    else {

    }
  }
}

double* test(Point* sites, int n, int* e_n) {
  Voronoi* v = v_new(sites, n);
  v_compute(v);
  *e_n = v->edges->next;

  double* edges = malloc(4*(*e_n)*sizeof(double));

  printf("\nEDGES\n");
  for (int i = 0; i < v->edges->next; i++) {
    Edge* e = v->edges->arr[i];
    disp_edge(e); 
    continue;
    // CLOSE EDGES 
    // sinon seg fault
    edges[4*i+0] = e->start->x;
    edges[4*i+1] = e->start->y;
    edges[4*i+2] = e->end->x;
    edges[4*i+3] = e->end->y;
    printf("(%f, %f), ", e->start->x, e->start->y);
    printf("(%f, %f)\n", e->end->x, e->end->y);
  }

  *e_n = 2*(*e_n);
  return edges;
}

Point* gen_sites(int n) {
  srand(time(NULL));
  Point* sites = malloc(n*sizeof(Point));
  for (int i = 0; i < n; i++) {
    double x = 100 * (double)rand()/RAND_MAX;
    double y = 100 * (double)rand()/RAND_MAX;
    Point p = { x, y };
    sites[i] = p;
  }
  return sites;
}

double* concat(double* a, double* b, int n, int m) {
  double* c = malloc((n+m)*sizeof(double));
  for (int i = 0; i < n; i++)
    c[i] = a[i];
  for (int i = 0; i < m; i++)
    c[i+n] = b[i];
  return c;
}