#include <string.h>
#include <stdlib.h>

#include <TFT_eSPI.h>
#include "pins2024.h"
#include "Input.hpp"
#include "Menu.hpp"

namespace Menu {
    Menu::Menu(TFT_eSPI &display, Input &inputHandler) 
        : tft(display), input(inputHandler) {
        title = nullptr;
        lastIndex = -1;
        selectedIndex = -1;
        titleColor = TFT_WHITE;
        itemColor = TFT_WHITE;
        selectedItemColor = TFT_BLACK;
        
        // Initialize items array
        for(int i = 0; i < MAX_MENU_ITEMS; i++) {
            items[i] = nullptr;
        }
    }

    void Menu::setTitle(const char *menuTitle) {
        // Free previous title if it exists
        if (title != nullptr) {
            free(title);
        }
        
        // Allocate and copy new title
        if (menuTitle != nullptr) {
            title = (char*)malloc(strlen(menuTitle) + 1);
            strcpy(title, menuTitle);
        } else {
            title = nullptr;
        }
    }

    void Menu::addItem(const char *item) {
        if (lastIndex >= MAX_MENU_ITEMS - 1 || item == nullptr) {
            return;
        }
        
        // Increment lastIndex and allocate memory for new item
        lastIndex++;
        items[lastIndex] = (char*)malloc(strlen(item) + 1);
        strcpy(items[lastIndex], item);
    }

    void Menu::clearItems() {
        // Free all allocated items
        for(int i = 0; i <= lastIndex; i++) {
            if (items[i] != nullptr) {
                free(items[i]);
                items[i] = nullptr;
            }
        }
        lastIndex = -1;
        selectedIndex = 0;
    }

    int Menu::getSelectedIndex() {
        return processInput();
    }

    void Menu::setColors(uint32_t newTitleColor, uint32_t newItemColor, uint32_t newSelectedItemColor) {
        titleColor = newTitleColor;
        itemColor = newItemColor;
        selectedItemColor = newSelectedItemColor;
    }

    void Menu::render(bool clearScreen) {
        // Clear the display
        if (clearScreen) tft.fillScreen(TFT_BLACK);

        int16_t y = 10; // Starting y position
        
        // Render title if it exists
        if (title != nullptr) {
            tft.setTextColor(titleColor);
            tft.setTextSize(2);
            tft.setCursor(tft.width() / 2 - tft.textWidth(title) / 2, y);
            tft.print(title);
            y += 30; // Move down for items
        }
        
        // Render menu items
        tft.setTextSize(2);
        for (int i = 0; i <= lastIndex; i++) {
            if (i == selectedIndex) {
                tft.setTextColor(selectedItemColor, itemColor);
            } else {
                tft.setTextColor(itemColor, selectedItemColor);
            }
            tft.setCursor(30, y);
            tft.print(items[i]);
            y += 20; // Move down for next item
        }
    }

    int Menu::processInput() {
        // Check for user input and update selectedIndex accordingly
        if (input.getJoyY() < -0.5f) {
            selectedIndex--;
            if (selectedIndex < 0) {
                selectedIndex = lastIndex;
            }
            // Render the updated menu
            render(false);
            delay(200); // Debounce delay
        } else if (input.getJoyY() > 0.5f) {
            selectedIndex++;
            if (selectedIndex > lastIndex) {
                selectedIndex = 0;
            }
            // Render the updated menu
            render(false);
            delay(200); // Debounce delay
        } else if (input.hasBeenPressed(PIN_A)) {
            // Confirm selection
            return selectedIndex;
        }
        return -1; // No selection made yet

    }

}
