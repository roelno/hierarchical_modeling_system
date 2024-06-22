#include "../include/viewing.h"
#include <math.h>
#include <stdio.h>

void view2D_set(View2D *view, Point *vrp, double dx, Vector *x, int screenx,
                int screeny) {
  point_copy(&view->vrp, vrp);
  vector_copy(&view->x, x);
  view->dx = dx;
  view->screenx = screenx;
  view->screeny = screeny;
}

void matrix_setView2D(Matrix *vtm, View2D *view) {
  matrix_identity(vtm);

  // Step 1: Translate the VRP to the origin
  // Since vrp is a point, its coordinates can be accessed with vrp.val
  matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
  matrix_print(vtm, stdout);

  // Step 2: Rotate to align with the x-axis
  double length =
      sqrt(view->x.val[0] * view->x.val[0] + view->x.val[1] * view->x.val[1]);
  double cth = view->x.val[0] / length; // cosine of the rotation angle
  double sth = view->x.val[1] / length; // sine of the rotation angle
  matrix_rotateZ(vtm, cth,
                 -sth);
  matrix_print(vtm, stdout);

  // Step 3: Scale the scene to fit the screen dimensions
  // The dx should fit across the width of the screen, and the height is scaled
  // with the same factor to maintain the aspect ratio
  double sx = view->screenx / view->dx;
  double sy = -sx;

  matrix_scale2D(vtm, sx, sy);
  matrix_print(vtm, stdout);

  matrix_translate2D(vtm, view->screenx / 2, view->screeny / 2);
  matrix_print(vtm, stdout);
}

void matrix_setView3D(Matrix *vtm, View3D *view) {
  Vector U;
  Vector VUP;
  Vector VPN;

  vector_copy(&VPN, &view->vpn);
  vector_cross(&view->vup, &view->vpn, &U);
  vector_cross(&view->vpn, &U, &VUP);

  // Initialize VTM to the identity matrix
  matrix_identity(vtm);

  // Step 1: Translate the world so that the VRP is at the origin
  matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1],
                   -view->vrp.val[2]);
  matrix_print(vtm, stdout);

  // Step 2: Align the axes
  // Calculate the orthonormal basis for the camera
  Vector u, v, w;
  vector_copy(&w, &VPN);
  vector_normalize(&w);

  vector_copy(&u, &U);
  vector_normalize(&u);        

  vector_copy(&v, &VUP);
  vector_normalize(&v);
  vector_print(&u, stdout);
  vector_print(&v, stdout);
  vector_print(&w, stdout);

  // Create the rotation matrix using u, v, w
  Matrix rotation;
  matrix_identity(&rotation);
  matrix_set(&rotation, 0, 0, u.val[0]);
  matrix_set(&rotation, 0, 1, u.val[1]);
  matrix_set(&rotation, 0, 2, u.val[2]);
  matrix_set(&rotation, 1, 0, v.val[0]);
  matrix_set(&rotation, 1, 1, v.val[1]);
  matrix_set(&rotation, 1, 2, v.val[2]);
  matrix_set(&rotation, 2, 0, w.val[0]);
  matrix_set(&rotation, 2, 1, w.val[1]);
  matrix_set(&rotation, 2, 2, w.val[2]);
  matrix_multiply(&rotation, vtm, vtm);
  matrix_print(vtm, stdout);

  matrix_translate(vtm, 0, 0, view->d);
  matrix_print(vtm, stdout);

  double B = view->b + view->d;
  matrix_scale(vtm, 2*view->d/(B*view->du), 2*view->d/(B*view->dv), 1/B);
  matrix_print(vtm, stdout);

  double D = view->d / B;
  Matrix per;
  matrix_identity(&per);
  matrix_set(&per, 3, 2, 1/D);
  matrix_set(&per, 3, 3, 0);
  matrix_multiply(&per, vtm, vtm);
  matrix_print(vtm, stdout);

  matrix_scale2D(vtm, -view->screenx/2/D, -view->screeny/2/D);
  matrix_print(vtm, stdout);
  matrix_translate(vtm, view->screenx/2, view->screeny/2, 0);
  matrix_print(vtm, stdout);
}
