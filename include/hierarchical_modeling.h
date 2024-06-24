#ifndef HIERARCHICAL_MODELING_H
#define HIERARCHICAL_MODELING_H

#include "graphics.h"
#include "transform.h"

// Object types Enum
typedef enum {
  ObjNone,
  ObjLine,
  ObjPoint,
  ObjPolyline,
  ObjPolygon,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  //   ObjLight,
  ObjModule
} ObjectType;

// Element structure
typedef struct Element {
  ObjectType type;
  void *obj;
  struct Element *next;
} Element;

// Module structure
typedef struct {
  Element *head;
  Element *tail;
} Module;

// ShadeMethod Enum
typedef enum {
  ShadeFrame,
  ShadeConstant,
  ShadeDepth,
  ShadeFlat,
  ShadeGouraud,
  ShadePhong
} ShadeMethod;

// DrawState structure
typedef struct {
  Color color;
  Color flatColor;
  Color body;
  Color surface;
  ShadeMethod shade;
  float surfaceCoeff;
  int zBufferFlag;
  Point viewer;
} DrawState;

// Lighting structure
typedef struct {
  int nLights;
} Lighting;

// Function to create an initialized but empty Element
Element *element_create();

// Allocate an Element and store a duplicate of the data pointed to by obj in
// the Element. Modules do not get duplicated. The function needs to handle each
// type of object separately in a case statement.
Element *element_init(ObjectType type, void *obj);

// free the element and the object it contains, as appropriate.
void element_delete(Element *e);

// Allocate an empty module.
Module *module_create();

// clear the module’s list of Elements, freeing memory as appropriate.
void module_clear(Module *md);

// Free all of the memory associated with a module, including the memory pointed
// to by md.
void module_delete(Module *md);

// Generic insert of an element into the module at the tail of the list.
void module_insert(Module *md, Element *e);

// Adds a pointer to the Module sub to the tail of the module’s list.
void module_module(Module *md, Module *sub);

// Adds p to the tail of the module’s list.
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);

// Object that sets the current transform to the identity, placed at the tail of
// the module’s list.
void module_identity(Module *md);

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty);

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy);

// Matrix operand to add a rotation about the Z axis to the tail of the module’s
// list
void module_rotateZ(Module *md, double cth, double sth);

// Matrix operand to add a 2D shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy);

// Draw the module into the image using the given view transformation matrix
// [VTM], Lighting and DrawState by traversing the list of Elements. (For now,
// Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds,
                 Lighting *lighting, Image *src);

// Matrix operand to add a 3D translation to the Module
void module_translate(Module *md, double tx, double ty, double tz);

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz);

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth);

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth);

// Matrix operand to add a rotation that orients to the orthonormal axes u, v, w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

//  Adds a unit cube, axis-aligned and centered on zero to the Module. If solid
//  is zero, add only lines. If solid is non-zero, use polygons. Make sure each
//  polygon has surface normals defined for it
void module_cube(Module *md, int solid);

// Adds the foreground color value to the tail of the module’s list.
void module_color(Module *md, Color *c);

//  Adds the body color value to the tail of the module’s list
void module_bodyColor(Module *md, Color *c);

// Adds the surface color value to the tail of the module’s list.
void module_surfaceColor(Module *md, Color *c);

// Adds the specular coefficient to the tail of the module’s list.
void module_surfaceCoeff(Module *md, float coeff);

// create a new DrawState structure and initialize the fields.
DrawState *drawstate_create();

// set the color field to c.
void drawstate_setColor(DrawState *ds, Color c);

// set the body field to c.
void drawstate_setBody(DrawState *ds, Color c);

// set the surface field to c.
void drawstate_setSurface(DrawState *ds, Color c);

// set the surfaceCoeff field to f.
void drawstate_setSurfaceCoeff(DrawState *ds, float f);

// copy the DrawState data.
void drawstate_copy(DrawState *to, DrawState *from);

#endif
