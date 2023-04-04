#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "voronoi.h"
#include "shaders.h"

#define SIZE 800

void test();
Point* gen_sites(int n);



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

  int n = 10;
  Point* sites = gen_sites(n);
  double* vertices = malloc(2*n*sizeof(double));
  for (int i = 0; i < n; i++) {
    vertices[2*i] = sites[i].x;
    vertices[2*i+1] = sites[i].y;
    //printf("(%f, %f)\n", sites[i].x, sites[i].y);
  }
  
  float m[] = {
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
  glBufferData(GL_ARRAY_BUFFER, n*2*sizeof(double), vertices, GL_STATIC_DRAW);

  GLuint program = createBasicProgram(vertex_source, fragment_source);

  glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), (void*)0);
  glEnableVertexAttribArray(0);

  GLint mLoc = glGetUniformLocation(program, "m");
  GLint colorLoc = glGetUniformLocation(program, "color");
  
  glPointSize(10.);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glUniformMatrix3fv(mLoc, 1, GL_TRUE, m);
    glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, n);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}



void test() {
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