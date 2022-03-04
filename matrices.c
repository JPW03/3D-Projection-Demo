#include "matrices.h"
#include <math.h>


// Multiplying a 3D position by a 4x4 Matrix 
Vector3 Matrix4MultiplyVector3(Matrix mat, Vector3 vec) {
    
    // There must be 4 values to multiply by a 4x4 matrix, so 1 will be used at the 4th value.
    
    //Visualisation
    //  x       m0  m4  m8  m12         
    //  y       m1  m5  m9  m13         
    //  z   *   m2  m6  m10 m14 
    //  1       m3  m7  m11 m15
    //      For a better explanation of matrices multiplication, see: https://www.mathsisfun.com/algebra/matrix-multiplying.html

    Vector3 result = (Vector3) {
        (vec.x * mat.m0) + (vec.y * mat.m1) + (vec.z * mat.m2) + (mat.m3),
        (vec.x * mat.m4) + (vec.y * mat.m5) + (vec.z * mat.m6) + (mat.m7),
        (vec.x * mat.m8) + (vec.y * mat.m9) + (vec.z * mat.m10) + (mat.m11)
    };

    // The matrix is 4x4, therefore a 4th result must be calculated
    float w = (vec.x * mat.m12) + (vec.y * mat.m13) + (vec.z * mat.m14) + (mat.m15);

    // To bring the 4th result down to a size 3 vector, divide each element by w
    if (w != 0.0f) {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }

    return result;
}


// Multiplying 2 matrices together
Matrix Matrix4MultiplyMatrix4(Matrix mat1, Matrix mat2) {
    Matrix result = (Matrix){
        //First row (first mat1 row * each mat2 column)
        (mat1.m0 * mat2.m0) + (mat1.m4 * mat2.m1) + (mat1.m8 * mat2.m2) + (mat1.m12 * mat2.m3),
        (mat1.m0 * mat2.m4) + (mat1.m4 * mat2.m5) + (mat1.m8 * mat2.m6) + (mat1.m12 * mat2.m7), 
        (mat1.m0 * mat2.m8) + (mat1.m4 * mat2.m9) + (mat1.m8 * mat2.m10) + (mat1.m12 * mat2.m11),
        (mat1.m0 * mat2.m12) + (mat1.m4 * mat2.m13) + (mat1.m8 * mat2.m14) + (mat1.m12 * mat2.m15), 

        //Second row (second mat1 row * each mat2 column)
        (mat1.m1 * mat2.m0) + (mat1.m5 * mat2.m1) + (mat1.m9 * mat2.m2) + (mat1.m13 * mat2.m3),
        (mat1.m1 * mat2.m4) + (mat1.m5 * mat2.m5) + (mat1.m9 * mat2.m6) + (mat1.m13 * mat2.m7), 
        (mat1.m1 * mat2.m8) + (mat1.m5 * mat2.m9) + (mat1.m9 * mat2.m10) + (mat1.m13 * mat2.m11),
        (mat1.m1 * mat2.m12) + (mat1.m5 * mat2.m13) + (mat1.m9 * mat2.m14) + (mat1.m13 * mat2.m15), 

        //Third row (second mat1 row * each mat2 column)
        (mat1.m2 * mat2.m0) + (mat1.m6 * mat2.m1) + (mat1.m10 * mat2.m2) + (mat1.m14 * mat2.m3),
        (mat1.m2 * mat2.m4) + (mat1.m6 * mat2.m5) + (mat1.m10 * mat2.m6) + (mat1.m14 * mat2.m7), 
        (mat1.m2 * mat2.m8) + (mat1.m6 * mat2.m9) + (mat1.m10 * mat2.m10) + (mat1.m14 * mat2.m11),
        (mat1.m2 * mat2.m12) + (mat1.m6 * mat2.m13) + (mat1.m10 * mat2.m14) + (mat1.m14 * mat2.m15), 

        //Fourth row (second mat1 row * each mat2 column)
        (mat1.m3 * mat2.m0) + (mat1.m7 * mat2.m1) + (mat1.m11 * mat2.m2) + (mat1.m15 * mat2.m3),
        (mat1.m3 * mat2.m4) + (mat1.m7 * mat2.m5) + (mat1.m11 * mat2.m6) + (mat1.m15 * mat2.m7), 
        (mat1.m3 * mat2.m8) + (mat1.m7 * mat2.m9) + (mat1.m11 * mat2.m10) + (mat1.m15 * mat2.m11),
        (mat1.m3 * mat2.m12) + (mat1.m7 * mat2.m13) + (mat1.m11 * mat2.m14) + (mat1.m15 * mat2.m15), 
    };
    return result;
}



// Rotation Matrices Construction
Matrix RotationX(float angleDegrees) {  //Rotating around the x-axis
    
    //Rotation via trigonometry is not processor efficient
    // Better alternatives include quaternions and Euler angles.

    //Trigonometric functions in code use radians not degrees
    float angleRadians = (angleDegrees * PI) / 180.0f;

    return (Matrix) {
        1.0f,   0.0f,                   0.0f,               0.0f,
        0.0f,   cosf(angleRadians),     sinf(angleRadians), 0.0f,
        0.0f,   -sinf(angleRadians),    cosf(angleRadians), 0.0f,
        0.0f,   0.0f,                   0.0f,               1.0f
    };
}

Matrix RotationY(float angleDegrees) {  //Rotating around the y-axis
    float angleRadians = (angleDegrees * PI) / 180.0f;

    return (Matrix) {
        cosf(angleRadians), 0.0f, sinf(angleRadians), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sinf(angleRadians), 0.0f, cosf(angleRadians), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix RotationZ(float angleDegrees) {  //Rotating around the z-axis
    float angleRadians = (angleDegrees * PI) / 180.0f;

    return (Matrix) {
        cosf(angleRadians), sinf(angleRadians), 0.0f, 0.0f,
        -sinf(angleRadians), cosf(angleRadians), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}