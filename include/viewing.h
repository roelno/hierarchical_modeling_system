#ifndef VIEWING_H
#define VIEWING_H

#include "point.h"
#include "transform.h"
typedef struct {
  Point vrp;
  Vector x;
  double dx;
  int screenx;
  int screeny;
} View2D;

typedef struct {
  Point vrp;
  Vector vpn;
  Vector vup;
  double d;
  double du;
  double dv;
  double f;
  double b;
  int screenx;
  int screeny;
} View3D;

// Sets vtm to be the view transformation defined by the 2DView structure.
void matrix_setView2D(Matrix *vtm, View2D *view);

// Implement the 3D perspective pipeline. When the function returns, the vtm
// should contain the complete view matrix. Inside the function, begin by
// initializing VTM to the identity. Do not modify any of the values in the
// PerspectiveView structure inside the function (no side-effects).
void matrix_setView3D(Matrix *vtm, View3D *view);


#endif // VIEWING_H