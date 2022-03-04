#pragma once
#include "include/raymath.h"
#include "include/raylib.h"


// Multiplying a 3D position by a 4x4 Matrix 
Vector3 Matrix4MultiplyVector3(Matrix mat, Vector3 vec);

// Multiplying 2 matrices together
Matrix Matrix4MultiplyMatrix4(Matrix mat1, Matrix mat2);



// Rotation Matrices Construction

// Note: Rotations using trigonometric functions are not efficient for larger 3D engines.

Matrix RotationX(float angleDegrees);
Matrix RotationY(float angleDegrees);
Matrix RotationZ(float angleDegrees);