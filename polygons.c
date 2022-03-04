#include "include/raylib.h"
#include "polygons.h"
#include "matrices.h"


Cube ConstructCube(Vector3 centre, float radius) {
    Cube newCube;

    //  Set the cube's centre
    newCube.centre = centre;

    //  Set the position of the cube
    newCube.vertices[0] = (Vector3){ centre.x - radius, centre.y - radius, centre.z - radius };
    newCube.vertices[1] = (Vector3){ centre.x + radius, centre.y - radius, centre.z - radius };
    newCube.vertices[2] = (Vector3){ centre.x - radius, centre.y + radius, centre.z - radius };
    newCube.vertices[3] = (Vector3){ centre.x + radius, centre.y + radius, centre.z - radius };
    newCube.vertices[4] = (Vector3){ centre.x - radius, centre.y - radius, centre.z + radius };
    newCube.vertices[5] = (Vector3){ centre.x + radius, centre.y - radius, centre.z + radius };
    newCube.vertices[6] = (Vector3){ centre.x - radius, centre.y + radius, centre.z + radius };
    newCube.vertices[7] = (Vector3){ centre.x + radius, centre.y + radius, centre.z + radius };

    //  Set connections between each vertex
    newCube.edges[0] = (Vector2){ 0.0f, 1.0f };
    newCube.edges[1] = (Vector2){ 0.0f, 2.0f };
    newCube.edges[2] = (Vector2){ 0.0f, 4.0f };
    newCube.edges[3] = (Vector2){ 1.0f, 3.0f };
    newCube.edges[4] = (Vector2){ 1.0f, 5.0f };
    newCube.edges[5] = (Vector2){ 2.0f, 3.0f };
    newCube.edges[6] = (Vector2){ 2.0f, 6.0f };
    newCube.edges[7] = (Vector2){ 3.0f, 7.0f };
    newCube.edges[8] = (Vector2){ 4.0f, 5.0f };
    newCube.edges[9] = (Vector2){ 4.0f, 6.0f };
    newCube.edges[10] = (Vector2){ 5.0f, 7.0f };
    newCube.edges[11] = (Vector2){ 6.0f, 7.0f };

    return newCube;
}


void CubeMultiplyMatrix(Cube* cubeSource, Matrix matrix) {
    //Apply to cube's centre
    cubeSource->centre = Matrix4MultiplyVector3(matrix, cubeSource->centre);
    
    //Apply to cube's vertices
    for (int v = 0; v < 8; v++) {
        cubeSource->vertices[v] = Matrix4MultiplyVector3(matrix, cubeSource->vertices[v]);
    }
}