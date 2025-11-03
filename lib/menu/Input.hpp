#pragma once
#include <Arduino.h>
#include "pins2024.h"

class Input {
public:
    static float getJoyX();
    static float getJoyY();
    static bool isDown(uint8_t pin);
    static bool hasBeenPressed(uint8_t pin);
    static void update(float deltatime);
    static void waitFor(uint8_t pin);
    static void init();
private:
    // Button state variables
    static bool A, A_prev;
    static bool B, B_prev;
    static bool X, X_prev;
    static bool Y, Y_prev;
    static bool MENU, MENU_prev;
    static bool START, START_prev;
    static bool JOY_X, JOY_X_prev;
    static bool JOY_Y, JOY_Y_prev;
};