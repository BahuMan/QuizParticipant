#pragma once

#include <TFT_eSPI.h>
#include "Input.hpp"
#define MAX_MENU_ITEMS 10

namespace Menu {
    class Menu {
        public:
            Menu(TFT_eSPI &display, Input &inputHandler);
            void setTitle(const char *menuTitle);
            void addItem(const char *item);
            void clearItems();
            int getSelectedIndex();
            void setColors(uint32_t titleColor, uint32_t itemColor, uint32_t selectedItemColor);
            void render(bool clearScreen = true);
        private:
            char *title;
            char *items[MAX_MENU_ITEMS];
            int lastIndex;
            int selectedIndex;
            int processInput();
            uint32_t titleColor;
            uint32_t itemColor;
            uint32_t selectedItemColor;
            TFT_eSPI &tft;
            Input &input;
    };
}
