#include <stdio.h>
#include <stdlib.h>
#include "../include/graphics.h"
#include "../include/viewing.h"
#include "../include/hierarchical_modeling.h"

Module *ship;

void cylinder( Module *mod, int sides );
void cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  for(i=0;i<sides;i++) {
    Point pt[4];

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    polygon_set( &p, 4, pt );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

void cube( Module *mod );
void cube( Module *mod ) {
  Point pt[4];
  Polygon p;

  polygon_init( &p );
  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2], -1,  1,  1 );
  point_set3D( &pt[3], -1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], 1, -1, -1 );
  point_set3D( &pt[1], 1, -1,  1 );
  point_set3D( &pt[2], 1,  1,  1 );
  point_set3D( &pt[3], 1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2],  1, -1,  1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, 1, -1 );
  point_set3D( &pt[1], -1, 1,  1 );
  point_set3D( &pt[2],  1, 1,  1 );
  point_set3D( &pt[3],  1, 1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1,  1, -1 );
  point_set3D( &pt[2],  1,  1, -1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, 1 );
  point_set3D( &pt[1], -1,  1, 1 );
  point_set3D( &pt[2],  1,  1, 1 );
  point_set3D( &pt[3],  1, -1, 1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

}

void sphere( Module *mod, int slices, int stacks, double radius ) {
    Polygon p;
    Point pt[4];
    double phi1, phi2, theta, deltaPhi, deltaTheta;
    int i, j;

    polygon_init( &p );
    deltaPhi = M_PI / stacks;
    deltaTheta = 2.0 * M_PI / slices;

    for (i = 0; i < stacks; i++) {
        phi1 = i * deltaPhi;
        phi2 = (i + 1) * deltaPhi;

        for (j = 0; j < slices; j++) {
            theta = j * deltaTheta;

            point_set3D( &pt[0], radius * sin(phi1) * cos(theta), radius * cos(phi1), radius * sin(phi1) * sin(theta) );
            point_set3D( &pt[1], radius * sin(phi2) * cos(theta), radius * cos(phi2), radius * sin(phi2) * sin(theta) );
            point_set3D( &pt[2], radius * sin(phi2) * cos(theta + deltaTheta), radius * cos(phi2), radius * sin(phi2) * sin(theta + deltaTheta) );
            point_set3D( &pt[3], radius * sin(phi1) * cos(theta + deltaTheta), radius * cos(phi1), radius * sin(phi1) * sin(theta + deltaTheta) );

            polygon_set( &p, 4, pt );
            module_polygon( mod, &p );
        }
    }

    polygon_clear( &p );
}

void cone( Module *mod, int sides, double height, double radius ) {
    Polygon p;
    Point top, base;
    double angle;
    int i;

    polygon_init( &p );
    point_set3D( &top, 0, height, 0 );

    // Fan for the base
    for (i = 0; i < sides; i++) {
        Point pt[3];

        angle = i * 2.0 * M_PI / sides;
        point_set3D( &pt[0], radius * cos(angle), 0, radius * sin(angle) );

        angle = (i + 1) % sides * 2.0 * M_PI / sides;
        point_set3D( &pt[1], radius * cos(angle), 0, radius * sin(angle) );

        point_copy( &pt[2], &top );

        polygon_set( &p, 3, pt );
        module_polygon( mod, &p );
    }

    polygon_clear( &p );
}

Module* create_spaceship(Module *ship) {
    Module *body, *engine, *cockpit, *thruster;
    Color Silver = { {0.75, 0.75, 0.75} };
    Color DarkGray = { {0.3, 0.3, 0.3} };
    Color LightBlue = { {0.5, 0.8, 0.9} };
    Color Red = { {0.9, 0.1, 0.1} };

    body = module_create();
    engine = module_create();
    cockpit = module_create();
    thruster = module_create();

    // Main body
    module_color(body, &Silver);
    module_scale(body, 1.0, 3.0, 1.0);
    cylinder(body, 20);
    module_module(ship, body);

    // Engine
    module_color(engine, &DarkGray);
    module_scale(engine, 0.7, 1.8, 0.7);
    module_translate(engine, 0, -2.0, 0); 
    cylinder(engine, 20);
    module_module(ship, engine);

    // Cockpit
    module_color(cockpit, &LightBlue);
    module_scale(cockpit, 0.8, 0.8, 0.8);
    module_translate(cockpit, 0, 1.5, 0);
    sphere(cockpit, 20, 20, 1);
    module_module(ship, cockpit);

    // Thruster (cone at the back)
    module_color(thruster, &Red);
    module_scale(thruster, 0.7, 1.5, 0.7);
    module_translate(thruster, 0, -4.7, 0);
    cone(thruster, 20, 1.5, 0.7);
    module_module(ship, thruster);

    return ship;
}

void create_formation(Module *mod, double tx, double ty, double tz, double angle) {
    int i;

    Module *formation = module_create();
    for (i = 0; i < 3; i++) {
        module_rotateX(formation, cos(angle), sin(angle));
        module_translate(formation, tx * i, ty * i, tz * (i + 1));
        module_module(formation, ship);
    }
    module_module(mod, formation);
}

int main(int argc, char *argv[]) {
    Image *src;
    Module *scene;
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;

    ship = module_create();
    create_spaceship(ship);    

    scene = module_create();

// Assuming create_formation and module_translate are properly defined and implemented elsewhere
for (int i = 0; i < 10; i++) {
    // Reset transformations to the scene's original state before applying new transformations
    module_identity(scene);

    // Apply translation
    module_translate(scene, i * 10 - 30, i % 3 - 1, (i % 2) * 5 - 5);

    // Create a formation with varying rotation based on the index
    create_formation(scene, 3 - (i % 3) * 2, i % 3 - 1, (i % 2) * 5 - 5, i * 10.0);
}

    // Set up the view
    point_set3D(&(view.vrp), 10, 10, 40);
    vector_set(&(view.vpn), 0, 0, -1);
    vector_set(&(view.vup), 0, 1, 0);
    view.d = 10;
    view.du = 20;
    view.dv = 15;
    view.f = 1;
    view.b = 100;
    view.screenx = 640;
    view.screeny = 360;

    matrix_setView3D(&vtm, &view);
    matrix_identity(&gtm);

    // Create the image and drawstate
    src = image_create(360, 640);
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    // Draw the scene
    module_draw(scene, &vtm, &gtm, ds, NULL, src);

    // Write out the scene
    image_write(src, "creative.ppm");

    // Clean up
    module_delete(scene);
    free(ds);
    image_free(src);

    return 0;
}