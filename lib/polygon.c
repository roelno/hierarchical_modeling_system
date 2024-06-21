#include "../include/polygon.h"
#include "../include/line.h"
#include <math.h>
#include <stdlib.h>

void *polygon_create() {
  Polygon *p = malloc(sizeof(Polygon));
  if (p == NULL) {
    return NULL;
  }

  p->oneSided = 0;
  p->nVertex = 0;
  p->vertex = NULL;
  return p;
}

void *polygon_createp(int numV, Point *vlist) {
  Polygon *p = malloc(sizeof(Polygon));
  if (p == NULL) {
    return NULL;
  }

  p->oneSided = 0;
  p->nVertex = numV;
  p->vertex = malloc(sizeof(Point) * numV);
  if (p->vertex == NULL) {
    free(p); // Free polygon if vertex allocation fails
    return NULL;
  }
  for (int i = 0; i < numV; i++) {
    point_copy(&p->vertex[i], &vlist[i]); // Copy each point
  }
  return p;
}

void polygon_free(Polygon *p) {
  if (p != NULL) {
    free(p->vertex); // Free the vertex array if it exists
    free(p);         // Free the polygon structure itself
  }
}

void polygon_init(Polygon *p) {
  if (p != NULL) {
    p->oneSided = 0;
    p->nVertex = 0;
    p->vertex = NULL; // Set vertex pointer to NULL
  }
}

void polygon_set(Polygon *p, int numV, Point *vlist) {
  if (p == NULL) {
    return;
  }

  free(p->vertex);
  p->vertex = malloc(sizeof(Point) * numV);
  if (p->vertex == NULL) {
    p->nVertex = 0; // Ensure consistent state on allocation failure
    return;
  }
  for (int i = 0; i < numV; i++) {
    point_copy(&p->vertex[i], &vlist[i]); // Copy new vertices
  }
  p->nVertex = numV; // Update vertex count
}

void polygon_clear(Polygon *p) {
  if (p != NULL) {
    free(p->vertex);  // Free the vertex array
    p->vertex = NULL; // Reset vertex pointer
    p->nVertex = 0;
    p->oneSided = 0;
  }
}

void polygon_setSided(Polygon *p, int oneSided) {
  if (p != NULL) {
    p->oneSided = oneSided;
  }
}

void polygon_copy(Polygon *to, Polygon *from) {
  if (to == NULL || from == NULL) {
    return;
  }

  to->oneSided = from->oneSided;
  to->nVertex = from->nVertex;

  free(to->vertex); // Free the existing vertices in 'to' if any
  to->vertex = malloc(sizeof(Point) * from->nVertex);
  if (to->vertex == NULL) {
    to->nVertex = 0; // Ensure consistency on allocation failure
    return;
  }
  for (int i = 0; i < from->nVertex; i++) {
    point_copy(&to->vertex[i], &from->vertex[i]); // Copy each vertex
  }
}

void polygon_print(Polygon *p, FILE *fp) {
  if (p == NULL || fp == NULL) {
    return;
  }

  fprintf(fp, "Polygon - One Sided: %s, Vertices: %d\n",
          (p->oneSided ? "Yes" : "No"), p->nVertex);
  for (int i = 0; i < p->nVertex; i++) {
    // fprintf(fp, "Vertex %d: ", i + 1);
    point_print(&p->vertex[i], fp);
  }
}

void polygon_normalize(Polygon *p) {
  if (p == NULL) {
    return;
  }
  for (int i = 0; i < p->nVertex; i++) {
    point_normalize(&p->vertex[i]);
  }
}

void polygon_draw(Polygon *p, Image *src, Color c) {
  if (p == NULL || src == NULL || p->nVertex < 2) {
    return; // Not enough vertices to form a line
  }

  Line l;
  for (int i = 0; i < p->nVertex - 1; i++) {
    line_set(&l, p->vertex[i], p->vertex[i + 1]);
    line_draw(&l, src, c);
  }
  // Connect the last vertex back to the first to close the polygon
  line_set(&l, p->vertex[p->nVertex - 1], p->vertex[0]);
  line_draw(&l, src, c);
}

void polygon_drawFillB(Polygon *p, Image *src, Color c) {
  if (p->nVertex != 3)
    return; // Ensure there are 3 vertices to form a triangle

  // Initialize bounding box values
  float minX = p->vertex[0].val[0], maxX = minX;
  float minY = p->vertex[0].val[1], maxY = minY;

  // Find the bounding box of the polygon
  for (int i = 1; i < p->nVertex; i++) {
    float vx = p->vertex[i].val[0], vy = p->vertex[i].val[1];
    if (vx < minX)
      minX = vx;
    if (vx > maxX)
      maxX = vx;
    if (vy < minY)
      minY = vy;
    if (vy > maxY)
      maxY = vy;
  }

  // Define vertices A, B, C
  float x1 = p->vertex[0].val[0], y1 = p->vertex[0].val[1];
  float x2 = p->vertex[1].val[0], y2 = p->vertex[1].val[1];
  float x3 = p->vertex[2].val[0], y3 = p->vertex[2].val[1];
  // printf("x1: %f, y1: %f, x2: %f, y2: %f, x3: %f, y3: %f\n", x1, y1, x2, y2,
  // x3, y3);

  // Denominators for beta and gamma
  float f_ca_beta = (y3 - y1) * x2 + (x1 - x3) * y2 + x3 * y1 - x1 * y3;
  float f_ab_gamma = (y1 - y2) * x3 + (x2 - x1) * y3 + x1 * y2 - x2 * y1;

  // Check if denominators are zero to avoid division by zero
  if (f_ca_beta == 0 || f_ab_gamma == 0)
    return; // Degenerate triangle (collinear vertices)

  // Scan over bounding box and check each pixel
  for (int y = (int)minY; y <= maxY; y++) {
    for (int x = (int)minX; x <= maxX; x++) {

      float px = x + 0.5; // Use center of the pixel
      float py = y + 0.5;

      // Calculate barycentric coordinates
      float beta =
          ((y3 - y1) * px + (x1 - x3) * py + x3 * y1 - x1 * y3) / f_ca_beta;
      float gamma =
          ((y1 - y2) * px + (x2 - x1) * py + x1 * y2 - x2 * y1) / f_ab_gamma;
      float alpha = 1.0f - beta - gamma;

      // Check if pixel is inside the triangle and all coordinates are within
      // [0, 1]
      if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 &&
          gamma <= 1) {
        image_setColor(src, y, x, c); // Function to set the color of the pixel
      }
      // printf("x: %d, y: %d, alpha: %f, beta: %f, gamma: %f\n", x, y, alpha,
      // beta, gamma);
    }
  }
}