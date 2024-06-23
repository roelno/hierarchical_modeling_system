#include "../include/hierarchical_modeling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create an initialized but empty Element
Element* element_create() {
    Element* new_element = (Element*)malloc(sizeof(Element));
    if (new_element == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_element->type = ObjNone;
    new_element->obj = NULL;
    new_element->next = NULL;
    return new_element;
}

// Function prototypes for object-specific duplication
void* duplicate_line(const Line* src);
void* duplicate_point(const Point* src);
void* duplicate_polyline(const Polyline* src);
void* duplicate_polygon(const Polygon* src);
void* duplicate_matrix(const Matrix* src);
void* duplicate_color(const Color* src);

// Allocate an Element and store a duplicate of the data pointed to by obj in
// the Element. Modules do not get duplicated. The function needs to handle each
// type of object separately in a case statement.
Element* element_init(ObjectType type, void* obj) {
    Element* new_element = element_create();
    new_element->type = type;
    switch (type) {
        case ObjNone:
            break;
        case ObjLine:
            new_element->obj = duplicate_line((Line*)obj);
            break;
        case ObjPoint:
            new_element->obj = duplicate_point((Point*)obj);
            break;
        case ObjPolyline:
            new_element->obj = duplicate_polyline((Polyline*)obj);
            break;
        case ObjPolygon:
            new_element->obj = duplicate_polygon((Polygon*)obj);
            break;
        case ObjIdentity:
            break;
        case ObjMatrix:
            new_element->obj = duplicate_matrix((Matrix*)obj);
            break;
        case ObjColor:
            new_element->obj = duplicate_color((Color*)obj);
            break;
        case ObjBodyColor:
            new_element->obj = duplicate_color((Color*)obj);
            break;
        case ObjSurfaceColor:
            new_element->obj = duplicate_color((Color*)obj);
            break;
        case ObjSurfaceCoeff: // coefficient is a float
            new_element->obj = malloc(sizeof(float));
            if (new_element->obj == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            memcpy(new_element->obj, obj, sizeof(float));
            break;
        // case ObjLight:
        //     break;
        case ObjModule:
            new_element->obj = obj; // do not duplicate module
            break;
        default:
            fprintf(stderr, "Invalid object type\n");
            exit(EXIT_FAILURE);
    }
    return new_element;
}

// Function to duplicate a Line object
void* duplicate_line(const Line* src) {
    Line* new_line = (Line*)malloc(sizeof(Line));
    if (new_line == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    line_copy(new_line, (Line*)src);
    return new_line;
}

// Function to duplicate a Point object
void* duplicate_point(const Point* src) {
    Point* new_point = (Point*)malloc(sizeof(Point));
    if (new_point == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    point_copy(new_point, (Point*)src);
    return new_point;
}

// Function to duplicate a Polyline object
void* duplicate_polyline(const Polyline* src) {
    Polyline* new_polyline = (Polyline*)malloc(sizeof(Polyline));
    if (new_polyline == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    polyline_copy(new_polyline, (Polyline*)src);
    return new_polyline;
}

// Function to duplicate a Polygon object
void* duplicate_polygon(const Polygon* src) {
    Polygon* new_polygon = (Polygon*)malloc(sizeof(Polygon));
    if (new_polygon == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    polygon_copy(new_polygon, (Polygon*)src);
    return new_polygon;
}

// Function to duplicate a Matrix object
void* duplicate_matrix(const Matrix* src) {
    Matrix* new_matrix = (Matrix*)malloc(sizeof(Matrix));
    if (new_matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    matrix_copy(new_matrix, (Matrix*)src);
    return new_matrix;
}

// Function to duplicate a Color object
void* duplicate_color(const Color* src) {
    Color* new_color = (Color*)malloc(sizeof(Color));
    if (new_color == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    color_copy(new_color, (Color*)src);
    return new_color;
}


// free the element and the object it contains, as appropriate.
void element_delete(Element* e) {
    if (e == NULL) {
        return;
    }

    switch (e->type) {
        case ObjNone:
            break;
        case ObjLine:
            free(e->obj);
            break;
        case ObjPoint:
            free(e->obj);
            break;
        case ObjPolyline:
            polyline_clear((Polyline*)e->obj);
            free(e->obj);
            break;
        case ObjPolygon:
            polygon_clear((Polygon*)e->obj);
            free(e->obj);
            break;
        case ObjIdentity:
            break;
        case ObjMatrix:
            matrix_clear((Matrix*)e->obj);
            free(e->obj);
            break;
        case ObjColor:
            free(e->obj);
            break;
        case ObjBodyColor:
            free(e->obj);
            break;
        case ObjSurfaceColor:
            free(e->obj);
            break;
        case ObjSurfaceCoeff:
            break;
        // case ObjLight:
        //     break;
        case ObjModule:
            // module_clear((Module*)e->obj);
            break;
        default:
            fprintf(stderr, "Invalid object type\n");
            exit(EXIT_FAILURE);
    }

    free(e);
}

// Allocate an empty module.
Module* module_create() {
    Module* new_module = (Module*)malloc(sizeof(Module));
    if (new_module == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_module->head = NULL;
    new_module->tail = NULL;
    return new_module;
}

// clear the module’s list of Elements, freeing memory as appropriate.
void module_clear(Module* md) {
    Element* current = md->head;
    while (current != NULL) {
        Element* next = current->next;
        element_delete(current);
        current = next;
    }
    md->head = NULL;
    md->tail = NULL;
}

// Free all of the memory associated with a module, including the memory pointed to by md.
void module_delete(Module* md) {
    module_clear(md);
    free(md);
}

// Generic insert of an element into the module at the tail of the list.
void module_insert(Module* md, Element* e) {
    if (md->head == NULL) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e;
        md->tail = e;
    }
}

// Adds a pointer to the Module sub to the tail of the module’s list.
void module_module(Module* md, Module* sub) {
    Element* new_element = element_create();
    new_element->type = ObjModule;
    new_element->obj = sub;
    module_insert(md, new_element);
}

void module_point(Module* md, Point* p) {
    Point* p_copy = duplicate_point(p);
    Element* new_element = element_create();
    new_element->type = ObjPoint;
    new_element->obj = p_copy;
    module_insert(md, new_element);
}

void module_line(Module* md, Line* l) {
    Line* l_copy = duplicate_line(l);
    Element* new_element = element_create();
    new_element->type = ObjLine;
    new_element->obj = l_copy;
    module_insert(md, new_element);
}

void module_polyline(Module* md, Polyline* p) {
    Polyline* p_copy = duplicate_polyline(p);
    Element* new_element = element_create();
    new_element->type = ObjPolyline;
    new_element->obj = p_copy;
    module_insert(md, new_element);
}

void module_polygon(Module* md, Polygon* p) {
    Polygon* p_copy = duplicate_polygon(p);
    Element* new_element = element_create();
    new_element->type = ObjPolygon;
    new_element->obj = p_copy;
    module_insert(md, new_element);
}

// Object that sets the current transform to the identity, placed at the tail of the module’s list.
void module_identity(Module* md) {
    Element* new_element = element_create();
    new_element->type = ObjIdentity;
    new_element->obj = NULL;
    module_insert(md, new_element);
}

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module* md, double tx, double ty) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_translate2D(m, tx, ty);

    Element* new_element = element_create();
    new_element->type = ObjMatrix;
    new_element->obj = m;
    module_insert(md, new_element);
}

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module* md, double sx, double sy) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_scale2D(m, sx, sy);

    Element* new_element = element_create();
    new_element->type = ObjMatrix;
    new_element->obj = m;
    module_insert(md, new_element);
}

// Matrix operand to add a rotation matrix to the tail of the module’s list.
void module_rotate2D(Module* md, double cth, double sth) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateZ(m, cth, sth);

    Element* new_element = element_create();
    new_element->type = ObjMatrix;
    new_element->obj = m;
    module_insert(md, new_element);
}

// Matrix operand to add a rotation about the Z axis to the tail of the module’s list
void module_rotateZ(Module* md, double cth, double sth) {
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateZ(m, cth, sth);

    Element* new_element = element_create();
    new_element->type = ObjMatrix;
    new_element->obj = m;
    module_insert(md, new_element);
}


// Helper function to apply transformations and draw a point
void draw_transformed_point(Point *p, Matrix *VTM, Matrix *GTM, Matrix *LTM, DrawState *ds, Image *src) {
    Point temp;
    matrix_xformPoint(LTM, p, &temp);    // LTM * Porg
    printf("LTM: \n");
    matrix_print(LTM, stdout);
    printf("temp: %f, %f, %f, %f\n", p->val[0], p->val[1], p->val[2], p->val[3]);
    matrix_xformPoint(GTM, &temp, &temp); // GTM * (LTM * Porg)
    printf("GTM: \n");
    matrix_print(GTM, stdout);
    printf("temp: %f, %f, %f, %f\n", temp.val[0], temp.val[1], temp.val[2], temp.val[3]);
    matrix_xformPoint(VTM, &temp, &temp); // VTM * (GTM * (LTM * Porg))
    printf("VTM: \n");
    matrix_print(VTM, stdout);
    printf("temp: %f, %f, %f, %f\n", temp.val[0], temp.val[1], temp.val[2], temp.val[3]);
    point_normalize(&temp);
    printf("norm temp: %f, %f, %f, %f\n", temp.val[0], temp.val[1], temp.val[2], temp.val[3]);
    point_draw(&temp, src, ds->color);
    printf("color: %f, %f, %f\n", ds->color.c[0], ds->color.c[1], ds->color.c[2]);
}


// Helper function to apply transformations and draw a line
void draw_transformed_line(Line *l, Matrix *VTM, Matrix *GTM, Matrix *LTM, DrawState *ds, Image *src) {
    Line temp;
    line_copy(&temp, l);
    matrix_xformPoint(LTM, &temp.a, &temp.a);
    matrix_xformPoint(LTM, &temp.b, &temp.b);
    matrix_xformPoint(GTM, &temp.a, &temp.a);
    matrix_xformPoint(GTM, &temp.b, &temp.b);
    matrix_xformPoint(VTM, &temp.a, &temp.a);
    matrix_xformPoint(VTM, &temp.b, &temp.b);
    line_draw(&temp, src, ds->color);
}

// Helper function to apply transformations and draw a polyline
void draw_transformed_polyline(Polyline *p, Matrix *VTM, Matrix *GTM, Matrix *LTM, DrawState *ds, Image *src) {
    Polyline temp;
    polyline_copy(&temp, p);
    matrix_xformPolyline(LTM, &temp);
    matrix_xformPolyline(GTM, &temp);
    matrix_xformPolyline(VTM, &temp);
    polyline_draw(&temp, src, ds->color);
}

// Helper function to apply transformations and draw a polygon
void draw_transformed_polygon(Polygon *p, Matrix *VTM, Matrix *GTM, Matrix *LTM, DrawState *ds, Image *src) {
    Polygon temp;
    polygon_copy(&temp, p);
    matrix_xformPolygon(LTM, &temp);
    matrix_xformPolygon(GTM, &temp);
    matrix_xformPolygon(VTM, &temp);
    polygon_drawFill(&temp, src, ds->color);
}

// Draw the module into the image using the given view transformation matrix
// [VTM], Lighting and DrawState by traversing the list of Elements. (For now,
// Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds,
                 Lighting *lighting, Image *src) {
    
    if (md == NULL || VTM == NULL || GTM == NULL || ds == NULL || src == NULL) {
        fprintf(stderr, "Error: NULL argument to module_draw\n");
        return;
    }

    Element* current = md->head;
    Matrix LTM, GTMpass;
    matrix_identity(&LTM);  // Initialize LTM to the identity matrix
    
    while (current != NULL) {
        switch (current->type) {
            case ObjNone:
                break;
            case ObjLine:
                draw_transformed_line((Line*)current->obj, VTM, GTM, &LTM, ds, src);                
                break;
            case ObjPoint:
                draw_transformed_point((Point*)current->obj, VTM, GTM, &LTM, ds, src);
                break;
            case ObjPolyline:
                draw_transformed_polyline((Polyline*)current->obj, VTM, GTM, &LTM, ds, src);
                break;
            case ObjPolygon:
                draw_transformed_polygon((Polygon*)current->obj, VTM, GTM, &LTM, ds, src);
                break;
            case ObjIdentity:
                matrix_identity(&LTM);
                break;
            case ObjMatrix:
                matrix_multiply((Matrix*)current->obj, &LTM, &LTM); // LTM = current->obj * LTM
                break;
            case ObjColor:
                break;
            case ObjBodyColor:
                break;
            case ObjSurfaceColor:
                break;
            case ObjSurfaceCoeff:
                break;
            // case ObjLight:
            //     break;
            case ObjModule:
                matrix_multiply(GTM, &LTM, &GTMpass);  // GTMpass = GTM * LTM
                DrawState* ds_copy = (DrawState*)malloc(sizeof(DrawState));
                if (ds_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                memcpy(ds_copy, ds, sizeof(DrawState));
                module_draw((Module*)current->obj, VTM, &GTMpass, ds_copy, NULL, src);
                free(ds_copy);
                break;
            default:
                fprintf(stderr, "Invalid object type\n");
                exit(EXIT_FAILURE);
        }
        current = current->next;
    }
}

// create a new DrawState structure and initialize the fields.
DrawState* drawstate_create() {
    DrawState* new_drawstate = (DrawState*)malloc(sizeof(DrawState));
    if (new_drawstate == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_drawstate->zBufferFlag = 1;
    new_drawstate->body = (Color){{0.0, 0.0, 0.0}};
    new_drawstate->surface = (Color){{0.0, 0.0, 0.0}};
    new_drawstate->color = (Color){{1.0, 1.0, 1.0}};
    new_drawstate->flatColor = (Color){{0.0, 0.0, 0.0}};
    new_drawstate->surface = (Color){{0.0, 0.0, 0.0}};
    new_drawstate->shade = ShadeConstant;
    new_drawstate->surfaceCoeff = 0.0;
    new_drawstate->viewer.val[0] = 0.0;
    new_drawstate->viewer.val[1] = 0.0;
    new_drawstate->viewer.val[2] = -1.0;
    return new_drawstate;
}

void drawstate_setColor(DrawState *ds, Color c) {
    ds->color = c;
}

void drawstate_setBody(DrawState *ds, Color c) {
    ds->body = c;
}

void drawstate_setSurface(DrawState *ds, Color c) {
    ds->surface = c;
}

void drawstate_setSurfaceCoeff(DrawState *ds, float f) {
    ds->surfaceCoeff = f;
}

void drawstate_copy(DrawState *to, DrawState *from) {
    to->color = from->color;
    to->flatColor = from->flatColor;
    to->body = from->body;
    to->surface = from->surface;
    to->shade = from->shade;
    to->surfaceCoeff = from->surfaceCoeff;
    to->zBufferFlag = from->zBufferFlag;
    to->viewer = from->viewer;
}