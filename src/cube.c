#include <stdio.h>
#include <stdlib.h>
#include "../include/graphics.h"
#include "../include/viewing.h"
#include "../include/hierarchical_modeling.h"


int main() {
    // draw an image of a 3-D blue cube outline, made using your module_cube function.
    Image *src;
    Module *cube;
    View3D view;
    Matrix VTM, GTM;
    DrawState *ds;
    Color blue;

    src = image_create(600, 600);
    ds = drawstate_create();
    point_set2D(&(view.vrp), 0.0, 0.0);
    point_set3D(&(view.vrp), 2.0, 2.5, 2.0);
    vector_set(&(view.vpn), -2.0, -2.5, -0.5);
    vector_set(&(view.vup), 0.0, 0.0, 1.0);
    view.d = 0.5;
    view.du = 2.0;
    view.dv = 2.0;
    view.f = 0.0;
    view.b = 10.0;
    view.screenx = 600;
    view.screeny = 600;
    matrix_setView3D(&VTM, &view);
    matrix_identity(&GTM);

    // Set the color to blue
    color_set(&blue, 0.0, 0.0, 1.0);
    drawstate_setColor(ds, blue);

    cube = module_create();
    module_cube(cube, 0);
    module_draw(cube, &VTM, &GTM, ds, NULL, src);

    image_write(src, "cube.ppm");
    image_free(src);
    module_delete(cube);
    free(ds);

    return(0);
}