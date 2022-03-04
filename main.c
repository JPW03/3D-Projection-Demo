#include <math.h>
#include <stdio.h>
#include <string.h>
#include "include/raylib.h"
#include "polygons.h"
#include "matrices.h"
#include "ui.h"



//https://www.raylib.com/cheatsheet/cheatsheet.html

//Note: projection debug console outputs are in the previous iteration in case I need to use them again.


//  -------------------------- Initialise global variables


// ----- WINDOW SETTINGS
int SCREEN_WIDTH = 1000;
int SCREEN_HEIGHT = 600;

Vector2 RENDER_POSITION = { 500.0f, 0.0f };
int RENDER_WIDTH = 500;
int RENDER_HEIGHT = 350;

float elapsedTime;


// ----- PROJECTION VARIABLES
int FIELD_OF_VIEW = 80;
float Z_NEAR = 10.0f;
float Z_FAR = 1000.f;
float AspectRatio;
float InvFOV;


// ----- 3D RENDER VARIABLES
Cube mainCube;
Vector3 cubePos;
Vector3 rateOfRotation = { 0.0f, 18.0f, 0.0f }; //Automatic rotation, in degrees per second
float CUBE_DEPTH = 30.f;


// ----- INPUT VARIABLES
bool lMousePressed = false;
bool rMousePressed = false;
Vector2 prevMousePos;
Vector2 currMousePos = { 0.0f, 0.0f };
Vector2 mouseRay;
bool mouseInRender = false;


// ----- FRUSTUM VARIABLES
float frustScale = 2.0f;





void DrawFrustum(Vector2 frustOrigin, float widthCoefficient) {

    //  Z_NEAR + Z_FAR = 1000; frustum height = 200
    //  Render to frustum scale: 1 to 2
    
    float frustWidth = 200 * widthCoefficient;
    float frustWidthNear = (Z_NEAR * frustScale) * widthCoefficient;
    
    Vector2 frustLeft = { frustOrigin.x - frustWidth, frustOrigin.y - 200 };            //Top left
    Vector2 frustRight = { frustOrigin.x + frustWidth, frustOrigin.y - 200 };           //Top right
    
    Vector2 frustLeftNear = { frustOrigin.x - frustWidthNear, frustOrigin.y - (Z_NEAR * frustScale) }; //Bottom left
    Vector2 frustRightNear = { frustOrigin.x + frustWidthNear, frustOrigin.y - (Z_NEAR * frustScale) };//Bottom right

    float lineThickness = 3;
    
    DrawLineEx(frustOrigin, frustLeftNear, lineThickness, GRAY);
    DrawLineEx(frustOrigin, frustRightNear, lineThickness, GRAY);

    DrawLineEx(frustLeftNear, frustLeft, lineThickness, DARKGRAY);
    DrawLineEx(frustRightNear, frustRight, lineThickness, DARKGRAY);

    DrawLineEx(frustRight, frustLeft, lineThickness, DARKGRAY);
    DrawLineEx(frustRightNear, frustLeftNear, lineThickness, DARKGRAY);

}







int main() {

    //Initialise window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Projection");
    SetTargetFPS(60);

    //Load font
    Font font20 = LoadFontEx("resources\\GOTHICB.TTF", 20, 0, NULL);
    Font font10 = LoadFontEx("resources\\GOTHICB.TTF", 10, 0, NULL);

    //Load textures
    Texture2D axisLabel = LoadTexture("resources\\axis.png");

    //Initialise projection settings
    float AspectRatio = (float)RENDER_HEIGHT / (float)RENDER_WIDTH;


    //Construct cube at (0, 0, 0), size 10
    Cube mainCube = ConstructCube((Vector3){ 0, 0, 0 }, 10);
    
    
    //Move the cube into the screen
    Matrix Translate = (Matrix){
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, CUBE_DEPTH, 0.f
    };
    CubeMultiplyMatrix(&mainCube, Translate);
    

    //Initialise UI elements
    Slider fovSlider;
    fovSlider.screenPos = (Vector2){ 140, 8 };
    fovSlider.length = 250;
    fovSlider.min = 30;
    fovSlider.max = 85;
    fovSlider.sliderPos = 137;

    Slider depthSlider;
    depthSlider.screenPos = (Vector2){ 140, 48 };
    depthSlider.length = 250;
    depthSlider.min = -5;
    depthSlider.max = 100;
    depthSlider.sliderPos = 131;

    Toggle matricesToggle;
    matricesToggle.screenPos = (Vector2){15,78};
    matricesToggle.state = false;

    //Main window loop
    while (!WindowShouldClose()) {

        //Get the elapsed time since the previous frame.
        elapsedTime = GetFrameTime();

        //Update mouse position variables
        prevMousePos = currMousePos;
        currMousePos = GetMousePosition();
        mouseRay = (Vector2){ prevMousePos.x - currMousePos.x, prevMousePos.y - currMousePos.y };
        //Note: Raylib 4.0's GetMouseDelta() could be used here instead

        //Check if the input is within the bounds of the render:
        mouseInRender = (currMousePos.x > RENDER_POSITION.x && currMousePos.y > 0 && currMousePos.y < RENDER_POSITION.y + RENDER_HEIGHT);


        // -------------------------- CHECK SLIDER INPUTS --------------------------

        //Reset Slider.selected bools
        fovSlider.selected = false;
        depthSlider.selected = false;

        //Update FoV slider
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !mouseInRender) {
            if (InSlider(fovSlider, prevMousePos)) {
                float newSliderPos = fovSlider.sliderPos - mouseRay.x;
                fovSlider.selected = true;

                //Only apply the new slider position if it is within the bar
                if (newSliderPos >= 0 && newSliderPos <= fovSlider.length) {
                    fovSlider.sliderPos = newSliderPos;
                }
                else if (newSliderPos < 0) {
                    fovSlider.sliderPos = 0;
                }
                else {
                    fovSlider.sliderPos = fovSlider.length;
                }
            }

            //Update depth of cube slider
            else if (InSlider(depthSlider, prevMousePos)) {
                float newSliderPos = round((double)(depthSlider.sliderPos - mouseRay.x));
                depthSlider.selected = true;

                //Only apply the new slider position if it is within the bar
                if (newSliderPos >= 0 && newSliderPos <= depthSlider.length) {
                    depthSlider.sliderPos = newSliderPos;
                }
                else if (newSliderPos < 0) {
                    depthSlider.sliderPos = 0;
                }
                else {
                    depthSlider.sliderPos = depthSlider.length;
                }
            }

            //Update matrices toggle
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && InToggle(matricesToggle, currMousePos)) {
                matricesToggle.state = abs(matricesToggle.state - 1);   //If 0, will = 1; if 1, will equal 0
            }
        }


        //  -------------------------- UPDATE VALUES FROM UI --------------------------

        FIELD_OF_VIEW = GetSliderValue(fovSlider);

        float invFOV = 1.0f / tanf( ( (FIELD_OF_VIEW * PI) / 180.0f ) / 2);

        Matrix projMatrix = {
            AspectRatio * invFOV,   0.f,        0.f,                                0.f,
            0.f,                    invFOV,     0.f,                                0.f,
            0.f,                    0.f,        Z_FAR / (Z_FAR - Z_NEAR),              1.f,
            0.f,                    0.f,        (-Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR),   0.f
        };



        //  -------------------------- TRANSFORM CUBE --------------------------

        //Store the cube's centre coordinates
        cubePos = mainCube.centre;


        //Translate cube to origin to prepare for rotation about the cube's axis.
        Matrix TranslateToOrigin = (Matrix){
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            -cubePos.x, -cubePos.y, -cubePos.z, 1.f
        };
        CubeMultiplyMatrix(&mainCube, TranslateToOrigin);


        //Allow transformations if the mouse is within the render
        if (mouseInRender) {


            // Check for left mouse button input
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) { //Process ongoing input
                
                //Rotate with respect to the y-axis based on change in x position
                CubeMultiplyMatrix(&mainCube, RotationY(-mouseRay.x));
                
                //Do same for x-axis based on change in y position
                CubeMultiplyMatrix(&mainCube, RotationX(-mouseRay.y));

            }


            // Check for right mouse button input
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) { //Process ongoing input

                Matrix Translate = (Matrix){
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    -(mouseRay.x * 0.1f), -(mouseRay.y * 0.1f), 0.f, 1.f
                };
                CubeMultiplyMatrix(&mainCube, Translate);

            }


            //Check for zooming in/out
            /*if (GetMouseWheelMove() < 0.f) {

                Matrix Translate = (Matrix){
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 1.0f, 1.f
                };
                CubeMultiplyMatrix(&mainCube, Translate);
                
            }
            else if (GetMouseWheelMove() > 0.f) {

                Matrix Translate = (Matrix){
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, -1.0f, 1.f
                };
                CubeMultiplyMatrix(&mainCube, Translate);

            }*/

        }


        //Automatically rotate the cube
        CubeMultiplyMatrix(&mainCube, RotationY(rateOfRotation.y * elapsedTime));

        //Update cube depth according to the slider
        cubePos.z -= CUBE_DEPTH;
        CUBE_DEPTH = GetSliderValue(depthSlider);
        cubePos.z += CUBE_DEPTH;

        //Restore the cube's position
        Matrix TranslateBack = (Matrix){
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            cubePos.x, cubePos.y, cubePos.z, 1.f
        };
        CubeMultiplyMatrix(&mainCube, TranslateBack);


        
        // Project the transformed cube:
        Cube cubeProjected = mainCube;

        CubeMultiplyMatrix(&cubeProjected, projMatrix);


        // -------------------------- Start Drawing --------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);



        // ---- DRAW THE EDGES OF THE CUBE

        for (int vCount = 0; vCount < (int)(sizeof cubeProjected.edges / sizeof cubeProjected.edges[0]); vCount++) {
            
            // Get the vertices of the edge
            Vector2 tempEdge = cubeProjected.edges[vCount];
            Vector3 vertex1 = cubeProjected.vertices[(int)tempEdge.x];
            Vector3 vertex2 = cubeProjected.vertices[(int)tempEdge.y];
            
            Vector3 vOriginal1 = mainCube.vertices[(int)tempEdge.x];
            Vector3 vOriginal2 = mainCube.vertices[(int)tempEdge.y];
            
            // Scale into the render view
            vertex1.x = ( (vertex1.x + 1) * 0.5 * RENDER_WIDTH ) + RENDER_POSITION.x;
            vertex1.y = ( (vertex1.y + 1) * 0.5 * RENDER_HEIGHT ) + RENDER_POSITION.y;
            vertex2.x = ( (vertex2.x + 1) * 0.5 * RENDER_WIDTH ) + RENDER_POSITION.x;
            vertex2.y = ( (vertex2.y + 1) * 0.5 * RENDER_HEIGHT ) + RENDER_POSITION.y;

            //If the vertex is too close to the camera, don't draw the line.
            if ( (vOriginal1.z > Z_NEAR && vOriginal2.z > Z_NEAR) ) {
                DrawLine(
                    vertex1.x, vertex1.y,
                    vertex2.x, vertex2.y,
                    RED);
            }
        }



        // ---- DRAW THE INDICES ABOVE EACH RESPECTIVE VERTEX

        for (int vCount = 0; vCount < (int)(sizeof cubeProjected.vertices / sizeof cubeProjected.vertices[0]); vCount++) {
            char vN = (vCount + 48);
            Vector3 vertex = cubeProjected.vertices[vCount];
            Vector3 vOriginal = mainCube.vertices[vCount];

            //If the vertex is too close to the camera, don't draw the label.
            if (vOriginal.z > Z_NEAR) {
                vertex.x = ( (vertex.x + 1) * 0.5 * RENDER_WIDTH ) + RENDER_POSITION.x;
                vertex.y = ( (vertex.y + 1) * 0.5 * RENDER_HEIGHT ) + RENDER_POSITION.y;
                DrawText(TextFormat("%c", vN), vertex.x, vertex.y - 15, 5, BLACK);
            }
        }




        //Draw box outside of the render area
        DrawRectangle(0, 0, SCREEN_WIDTH - RENDER_WIDTH, SCREEN_HEIGHT, WHITE);
        DrawRectangle(RENDER_POSITION.x, RENDER_POSITION.y + RENDER_HEIGHT, RENDER_WIDTH, SCREEN_HEIGHT - RENDER_HEIGHT, WHITE);

        //Draw render resolution display
        DrawTextEx(font20, TextFormat("%dx%d", RENDER_WIDTH, RENDER_HEIGHT), (Vector2){ 920, 4 }, 20, 1.2, BLACK);
        
        
        

        // ---- DRAW X-PLANE FRUSTUM

        //  Z_NEAR + Z_FAR = 1000; frustum height = 200
        //  Render to frustum scale: 1 to 2
        
        Vector2 xFrustOrigin = { 290.f, RENDER_HEIGHT + 225.f };
        float xFrustCoefficient = (1 / AspectRatio) * tanf( (FIELD_OF_VIEW * PI / 180.f) / 2 );   //Equal to the inverse of projMatrix.m0

        DrawFrustum(xFrustOrigin, xFrustCoefficient);

        //Draw the vertices of the cube into the frustum
        for (int vCount = 0; vCount < (int)(sizeof mainCube.vertices / sizeof mainCube.vertices[0]); vCount++) {
            Vector2 vertex = (Vector2) { (xFrustOrigin.x) + (mainCube.vertices[vCount].x * frustScale), xFrustOrigin.y - (mainCube.vertices[vCount].z * frustScale)};
            //Check the vertex isn't too far away from the frustum
            if (vertex.y > RENDER_HEIGHT) {
                DrawCircleV(vertex, 3, RED);
            }
        }



        // ---- DRAW Y-PLANE FRUSTUM

        Vector2 yFrustOrigin = { 780.f, RENDER_HEIGHT + 225.f };
        float yFrustCoefficient = tanf( (FIELD_OF_VIEW * PI / 180.f) / 2 );   //Equal to the inverse of projMatrix.m5

        DrawFrustum(yFrustOrigin, yFrustCoefficient);

        //Draw the vertices of the cube into the frustum
        for (int vCount = 0; vCount < (int)(sizeof mainCube.vertices / sizeof mainCube.vertices[0]); vCount++) {
            Vector2 vertex = (Vector2) { (yFrustOrigin.x) + (mainCube.vertices[vCount].y * frustScale), yFrustOrigin.y - (mainCube.vertices[vCount].z * frustScale)};

            //Check the vertex isn't too far away from the frustum
            if (vertex.y > RENDER_HEIGHT) {
                DrawCircleV(vertex, 3, RED);
            }
        }

        
        // ---- DRAW UI ELEMENTS

        DrawTextEx(font20, "Field of View", (Vector2){ 9, 8 }, 20, 1.2, BLACK);
        DrawSlider(fovSlider);
        DrawTextEx(font20, TextFormat("%d degrees", FIELD_OF_VIEW), (Vector2){ 400, 8 }, 20, 1.2, BLACK);

        DrawTextEx(font20, "Depth of Cube", (Vector2){ 9, 45 }, 20, 1.2, BLACK);
        DrawSlider(depthSlider);
        DrawTextEx(font20, TextFormat("~%d", (int)CUBE_DEPTH), (Vector2){ 400, 45 }, 20, 1.2, BLACK);

        DrawToggle(matricesToggle);
        DrawTextEx(font20, "View as Matrices", (Vector2){ 42, 75 }, 20, 1.2, BLACK);

        DrawLineEx((Vector2){ 0, 107 }, (Vector2){ (float)(SCREEN_WIDTH - RENDER_WIDTH), 103}, 7, RAYWHITE);


        if (matricesToggle.state == false) {

            //FoV Scale Factor:   1      = invFOV
            //                  tan(fov)
            DrawTextEx(font20, TextFormat("FoV Scale Factor:     1              ="), (Vector2){ 9,118 }, 20, 1.2, BLACK);
            DrawLineEx((Vector2){166,140}, (Vector2){257,140}, 2.f, BLACK);
            DrawTextEx(font20, TextFormat("tan(%d / 2)", FIELD_OF_VIEW), (Vector2){ 169,142 }, 20, 1.2, RED);
            DrawTextEx(font20, TextFormat("%f", invFOV), (Vector2){ 283,118 }, 20, 1.2, RED);

            //Aspect Ratio: width  = AspectRatio
            //              height
            DrawTextEx(font20, TextFormat("Aspect Ratio:          ="), 
            (Vector2){ 9, 170 }, 20, 1.2, BLACK);
            DrawTextEx(font20, TextFormat("%d", RENDER_HEIGHT), 
            (Vector2){ 127, 170 }, 20, 1.2, GREEN);
            DrawTextEx(font20, TextFormat("%d", RENDER_WIDTH), 
            (Vector2){ 127, 194 }, 20, 1.2, GREEN);
            DrawLineEx((Vector2){126,191}, (Vector2){167,191}, 2.f, BLACK);
            DrawTextEx(font20, "0.7", 
            (Vector2){ 188, 170 }, 20, 1.2, GREEN);

            //Depth Scale Factor:  1   = (1/CUBE_DEPTH)
            //                    z
            DrawTextEx(font20, TextFormat("Depth Scale Factor:      1            ="), 
            (Vector2){ 9, 215 }, 20, 1.2, BLACK);
            DrawLineEx((Vector2){185,237}, (Vector2){272,237}, 2.f, BLACK);
            DrawTextEx(font20, TextFormat("%f", CUBE_DEPTH), 
            (Vector2){ 185, 239 }, 20, 1.2, PURPLE);
            DrawTextEx(font20, TextFormat("%f", (1 / CUBE_DEPTH)), 
            (Vector2){ 303, 215 }, 20, 1.2, PURPLE);

        }
        else {

            //Draw projection matrix
            DrawTextEx(font20, "x\ny\nz", 
            (Vector2){ 15, 124 }, 20, 1.2, BLACK);

            //First row
            DrawTextEx(font20, TextFormat("%d", RENDER_HEIGHT), (Vector2){ 100, 121 }, 20, 1.2, GREEN);
            DrawLineEx((Vector2){100,143}, (Vector2){129,143}, 2.f, BLACK);
            DrawTextEx(font20, TextFormat("%d", RENDER_WIDTH), (Vector2){ 100, 145 }, 20, 1.2, GREEN);
            DrawCircle(139, 142, 3, BLACK);
            DrawTextEx(font20, "1", (Vector2){ 169, 121 }, 20, 1.2, RED);
            DrawLineEx((Vector2){148,143}, (Vector2){232,143}, 2.f, BLACK);
            DrawTextEx(font20, TextFormat("tan(%d / 2)", FIELD_OF_VIEW), (Vector2){ 147,145 }, 20, 1.2, RED);

            DrawTextEx(font20, "0", (Vector2){ 299, 135 }, 20, 1.2, BLACK);

            DrawTextEx(font20, "0", (Vector2){ 404, 135 }, 20, 1.2, BLACK);

            //Second row
            DrawTextEx(font20, "0", (Vector2){ 136, 190 }, 20, 1.2, BLACK);

            DrawTextEx(font20, "1", (Vector2){ 275, 174 }, 20, 1.2, RED);
            DrawLineEx((Vector2){254,196}, (Vector2){338,196}, 2.f, BLACK);
            DrawTextEx(font20, TextFormat("tan(%d / 2)", FIELD_OF_VIEW), (Vector2){ 253,198 }, 20, 1.2, RED);

            DrawTextEx(font20, "0", (Vector2){ 404, 190 }, 20, 1.2, BLACK);

            //Third row
            DrawTextEx(font20, "0", (Vector2){ 136, 237 }, 20, 1.2, BLACK);
            DrawTextEx(font20, "0", (Vector2){ 299, 237 }, 20, 1.2, BLACK);
            DrawTextEx(font20, "1", (Vector2){ 404, 237 }, 20, 1.2, PURPLE);

            //Draw brackets
            DrawLineEx((Vector2){6,128}, (Vector2){6,202}, 3.f, BLACK);
            DrawLineEx((Vector2){31,128}, (Vector2){31,202}, 3.f, BLACK);
            DrawLineEx((Vector2){6,129.5}, (Vector2){10,129.5}, 3.f, BLACK);
            DrawLineEx((Vector2){31,129.5}, (Vector2){27,129.5}, 3.f, BLACK);
            DrawLineEx((Vector2){6,200.5}, (Vector2){10,200.5}, 3.f, BLACK);
            DrawLineEx((Vector2){31,200.5}, (Vector2){27,200.5}, 3.f, BLACK);

            DrawCircle(59, 162, 3, BLACK);

            DrawLineEx((Vector2){94,119}, (Vector2){94,263}, 3.f, BLACK);
            DrawLineEx((Vector2){423,119}, (Vector2){423,263}, 3.f, BLACK);
            DrawLineEx((Vector2){94,120.5}, (Vector2){98,120.5}, 3.f, BLACK);
            DrawLineEx((Vector2){423,120.5}, (Vector2){419,120.5}, 3.f, BLACK);
            DrawLineEx((Vector2){94,261.5}, (Vector2){98,261.5}, 3.f, BLACK);
            DrawLineEx((Vector2){423,261.5}, (Vector2){419,261.5}, 3.f, BLACK);

        }

        //Coordinate Projection Example
        // [x, y, z] -> [ ----x ]
        //              [ ----y ]
        DrawTextEx(font20, "[x, y, z] -> ", (Vector2){ 9, 280 }, 20, 1.2, BLACK);
        DrawLineEx((Vector2){100,282}, (Vector2){100,326}, 3.f, BLACK);
        DrawLineEx((Vector2){104,283.5}, (Vector2){100,283.5}, 3.f, BLACK);
        DrawLineEx((Vector2){104,324.5}, (Vector2){100,324.5}, 3.f, BLACK);
        
        DrawTextEx(font20, "(0.7)", (Vector2){ 106, 280 }, 20, 1.2, GREEN);
        DrawTextEx(font20, TextFormat("(%f)", invFOV), (Vector2){ 149, 280 }, 20, 1.2, RED);
        DrawTextEx(font20, TextFormat("(%f)", (1/CUBE_DEPTH)), (Vector2){ 244, 280 }, 20, 1.2, PURPLE);
        DrawTextEx(font20, "x", (Vector2){ 337, 280 }, 20, 1.2, BLACK);

        DrawTextEx(font20, TextFormat("(%f)", invFOV), (Vector2){ 149, 303 }, 20, 1.2, RED);
        DrawTextEx(font20, TextFormat("(%f)", (1/CUBE_DEPTH)), (Vector2){ 244, 303 }, 20, 1.2, PURPLE);
        DrawTextEx(font20, "y", (Vector2){ 337, 303 }, 20, 1.2, BLACK);

        DrawLineEx((Vector2){351,282}, (Vector2){351,326}, 3.f, BLACK);
        DrawLineEx((Vector2){347,283.5}, (Vector2){351,283.5}, 3.f, BLACK);
        DrawLineEx((Vector2){347,324.5}, (Vector2){351,324.5}, 3.f, BLACK);


        //Frustum axis labels

        // x-z frustum
        DrawTexture(axisLabel, 20, (int)xFrustOrigin.y - 82, BLACK);
        DrawTextEx(font20, "z", (Vector2){ 25, 472 }, 20, 1.2, BLACK);
        DrawTextEx(font20, "x", (Vector2){ 108, 557 }, 20, 1.2, BLACK);

        // y-z frustum
        DrawTexture(axisLabel, 589, (int)yFrustOrigin.y - 82, BLACK);
        DrawTextEx(font20, "z", (Vector2){ 594, 472 }, 20, 1.2, BLACK);
        DrawTextEx(font20, "y", (Vector2){ 676, 557 }, 20, 1.2, BLACK);


        //Frustum title label
        DrawLineEx((Vector2){0,(float)RENDER_HEIGHT - 3.5f}, 
        (Vector2){(float)(SCREEN_WIDTH - RENDER_WIDTH),(float)RENDER_HEIGHT - 3.5f}, 7.f, RAYWHITE);
        DrawTextEx(font20, "Viewing Frustum", 
        (Vector2){ 7, 353 }, 20, 1.2, BLACK);


        EndDrawing();
        // -------------------------- End Drawing --------------------------
    }

    CloseWindow();
}