#include "ui.h"
#include <stdio.h>


// --- CONSTANTS
// "slider" referring to the part the user can move back and forth
// "bar" referring to the longer rectangle part
float SLIDER_WIDTH = 16;
float SLIDER_HEIGHT = 30;
float SLIDER_BAR_HEIGHT = 16;

float TOGGLE_LENGTH = 16;



//Returns the current value of the position on the slider
float GetSliderValue(Slider slider) {
    return slider.min + ((slider.max - slider.min) * (slider.sliderPos / slider.length));
}


//Draws the slider to the screen (only works if BeginDrawing() has been called)
void DrawSlider(Slider slider) {
    float borderSize = 2;
    Color borderColor = DARKGRAY;
    Color fillColor = SKYBLUE;

    //Draw bar border
    DrawRectangleLinesEx((Rectangle) { slider.screenPos.x - borderSize, slider.screenPos.y - borderSize, slider.length + 6, SLIDER_BAR_HEIGHT + (2 * borderSize) }, borderSize, borderColor);

    //Fill the bar before the slider
    DrawRectangleV(slider.screenPos, (Vector2){ slider.sliderPos, SLIDER_BAR_HEIGHT }, fillColor);

    //Draw the slider (the part the user can move)
    DrawRectangle(((int)slider.screenPos.x + (int)slider.sliderPos) - (SLIDER_WIDTH / 2), (int)slider.screenPos.y - ((SLIDER_HEIGHT - SLIDER_BAR_HEIGHT) / 2), SLIDER_WIDTH, SLIDER_HEIGHT, GRAY);

    //Draw a border around the slider to show that it is selectable
    if (slider.selected) {
        DrawRectangleLinesEx((Rectangle) { ((int)slider.screenPos.x + (int)slider.sliderPos) - (SLIDER_WIDTH / 2), (int)slider.screenPos.y - ((SLIDER_HEIGHT - SLIDER_BAR_HEIGHT) / 2), SLIDER_WIDTH, SLIDER_HEIGHT }, borderSize, (Color){ 200, 200, 200, 255 });
    }
}


//Returns true if the position is within the slider
bool InSlider(Slider slider, Vector2 position) {
    if (position.x > ((int)slider.screenPos.x + (int)slider.sliderPos) - (SLIDER_WIDTH / 2) &&
    position.x < ((int)slider.screenPos.x + (int)slider.sliderPos) + (SLIDER_WIDTH / 2) &&
    position.y > (int)slider.screenPos.y - ((SLIDER_HEIGHT - SLIDER_BAR_HEIGHT) / 2) &&
    position.y < (int)slider.screenPos.y + SLIDER_HEIGHT - ((SLIDER_HEIGHT - SLIDER_BAR_HEIGHT) / 2) ) {
        slider.selected = true;
        return true;
    }
    else {
        slider.selected = false;
        return false;
    }
}



void DrawToggle(Toggle toggle) {
    float borderSize = 2;
    Color tColor = DARKGRAY;

    DrawRectangleRoundedLines((Rectangle){ toggle.screenPos.x, toggle.screenPos.y, TOGGLE_LENGTH, TOGGLE_LENGTH }, 1, 6, borderSize, tColor);

    if (toggle.state) {
        DrawRectangleRounded((Rectangle){ toggle.screenPos.x, toggle.screenPos.y, TOGGLE_LENGTH, TOGGLE_LENGTH }, 1, 6, SKYBLUE);
    }
}


bool InToggle(Toggle toggle, Vector2 position) {
    if (position.x > toggle.screenPos.x && position.x < toggle.screenPos.x + TOGGLE_LENGTH &&
    position.y > toggle.screenPos.y && position.y < toggle.screenPos.y + TOGGLE_LENGTH) {
        return true;
    }
    else {
        return false;
    }
}