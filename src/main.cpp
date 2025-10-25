#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite square = TFT_eSprite(&tft);

void setupDisplay() {
    tft.setRotation(4); //setRotation also sets some flags inside the AdaFruit drivers. After that, we manually set MADCTL
    tft.init(TFT_BLACK);
    tft.writecommand(TFT_MADCTL);
    tft.writedata(TFT_MAD_BGR | TFT_MAD_MV); //exchange red and blue bytes, and mirror x-coordinates
    tft.fillScreen(TFT_BLACK);

    square.createSprite(100, 100);
    square.fillSprite(TFT_BLACK);
    square.fillRect(0, 0, 45, 45, TFT_RED);
    square.fillRect(55, 0, 45, 45, TFT_GREEN);
    square.fillRect(0, 55, 45, 45, TFT_BLUE);
    square.fillRect(55, 55, 45, 45, TFT_YELLOW);
}

void setup() {
    // Initialize serial communication at 112500 bits per second
    Serial.begin(112500);
    setupDisplay();
}

void showDiamondColors();

void loop() {
    tft.setCursor(10, 50);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Hello, World!");
    Serial.println("Hello, World!");
    delay(2000);
    showDiamondColors();
    delay(2000);
}

void showDiamondColors() {
    tft.fillScreen(TFT_BLACK);
    tft.setPivot(200, 150);
    square.pushRotated(45);
    tft.setTextColor(TFT_BLACK, TFT_RED);
    tft.setCursor(200 - 5, 150 - 40);
    tft.println("X");
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.setCursor(200 + 30, 150 - 10);
    tft.println("A");
    tft.setTextColor(TFT_BLACK, TFT_BLUE);
    tft.setCursor(200 - 40, 150 - 10);
    tft.println("Y");
    tft.setTextColor(TFT_BLACK, TFT_YELLOW);
    tft.setCursor(200 - 5, 150 + 30);
    tft.println("B");
}