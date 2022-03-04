#pragma once
#include "include/raylib.h"

//  In 3D rendering, every shape is made up of triangles

typedef struct {
    Vector3 centre;
    Vector3 vertices[8];
    Vector2 edges[12];
} Cube;


//  Functions
Cube ConstructCube(Vector3 centre, float radius);

void CubeMultiplyMatrix(Cube* cube, Matrix mat);

