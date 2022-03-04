#include "include/raylib.h"

// --- DEFINE UI ELEMENTS AS STRUCTS

typedef struct {
    Vector2 screenPos;  //Position of the slider bar on the screen.
    float min;
    float max;
    float sliderPos;    //Central position of the slider within the bar.
    float length;       //Visible length of the slider
    bool selected;      //True if the mouse cursor is over the slider
} Slider;

typedef struct {
    Vector2 screenPos;
    bool state;
    bool selected;
} Toggle;


// --- DECLARE UI FUNCTIONS

float GetSliderValue(Slider slider);    //Returns the current value of the position on the slider
void DrawSlider(Slider slider);
bool InSlider(Slider slider, Vector2 position);        //Checks if a position is touching the sider


void DrawToggle(Toggle toggle);
bool InToggle(Toggle toggle, Vector2 position);