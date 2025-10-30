#include <Arduino.h>
#include "Input.hpp"
//#include <string.h>

// Define static variables
bool Input::A = false, Input::A_prev = false;
bool Input::B = false, Input::B_prev = false;
bool Input::X = false, Input::X_prev = false;
bool Input::Y = false, Input::Y_prev = false;
bool Input::MENU = false, Input::MENU_prev = false;
bool Input::START = false, Input::START_prev = false;
bool Input::JOY_X = false, Input::JOY_X_prev = false;
bool Input::JOY_Y = false, Input::JOY_Y_prev = false;

float Input::getJoyX() {
    return analogRead(PIN_JOY_X) / 4095.0f * 2.0f - 1.0f; // Normalize to -1.0 to 1.0
}
float Input::getJoyY() {
    return analogRead(PIN_JOY_Y) / 4095.0f * -2.0f + 1.0f; // Normalize to -1.0 to 1.0
}
bool Input::isDown(uint8_t pin) {
    switch(pin) {
        case PIN_A: return A;
        case PIN_B: return B;
        case PIN_X: return X;
        case PIN_Y: return Y;
        case PIN_MENU: return MENU;
        case PIN_START: return START;
        case PIN_JOY_X: return JOY_X;
        case PIN_JOY_Y: return JOY_Y;
        default: return false;
    }
}

bool Input::hasBeenPressed(uint8_t pin) {
    bool result = false;
    switch(pin) {
        case PIN_A: result = A_prev; A_prev = false; break;
        case PIN_B: result = (B && !B_prev); break;
        case PIN_X: result = (X && !X_prev); break;
        case PIN_Y: result = (Y && !Y_prev); break;
        case PIN_MENU: result = (MENU && !MENU_prev); break;
        case PIN_START: result = (START && !START_prev); break;
        case PIN_JOY_X: result = (JOY_X && !JOY_X_prev); break;
        case PIN_JOY_Y: result = (JOY_Y && !JOY_Y_prev); break;
        default: result = false;
    }
    return result;
}

void Input::waitFor(uint8_t pin) {
    while (!isDown(pin)) {
        update(0.1f); // Update input state
        delay(100); // Debounce delay
    }
    while (isDown(pin)) {
        update(0.1f); // Update input state
        delay(100); // Debounce delay
    }
}

void Input::update(float deltatime) {

    // Poll digital pins for buttons (pullup: LOW = pressed)
    A_prev |= A = !digitalRead(PIN_A);
    B_prev |= B = !digitalRead(PIN_B);
    X_prev |= X = !digitalRead(PIN_X);
    Y_prev |= Y = !digitalRead(PIN_Y);
    MENU_prev |= MENU = !digitalRead(PIN_MENU);
    START_prev |= START = !digitalRead(PIN_START);
    // For joystick, treat as button pressed if far from center (example threshold)
    JOY_X_prev |= JOY_X = (abs(analogRead(PIN_JOY_X) - 2048) > 512);
    JOY_Y_prev |= JOY_Y = (abs(analogRead(PIN_JOY_Y) - 2048) > 512);
}

void Input::init() {
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_B, INPUT_PULLUP);
    pinMode(PIN_X, INPUT_PULLUP);
    pinMode(PIN_Y, INPUT_PULLUP);
    pinMode(PIN_MENU, INPUT_PULLUP);
    pinMode(PIN_START, INPUT_PULLUP);
    pinMode(PIN_JOY_X, INPUT);
    pinMode(PIN_JOY_Y, INPUT);
    // Initialize state variables
    A = A_prev = false;
    B = B_prev = false;
    X = X_prev = false;
    Y = Y_prev = false;
    MENU = MENU_prev = false;
    START = START_prev = false;
    JOY_X = JOY_X_prev = false;
    JOY_Y = JOY_Y_prev = false;
}