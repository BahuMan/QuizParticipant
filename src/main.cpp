#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
//this file should define WIFI_SSID and WIFI_PWD
//it is not included in the repository for security reasons
#include "password.h"
#include <PubSubClient.h>
#include <ESP32Ping.h>

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite square = TFT_eSprite(&tft);
IPAddress mqttServer(192,168,0,156);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

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

bool setupWifi() {
    // Connect to WiFi
    tft.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        tft.print(".");
    }
    tft.println("\nWiFi connected!");
    tft.print("IP address: ");
    tft.println(WiFi.localIP());
    return true;
}

void setupMqtt() {
    mqttClient.setServer(mqttServer, 1883);
    tft.println("MQTT configured");
}

bool connectMqtt() {
    if (!mqttClient.connected()) {
        // Ping MQTT server before attempting connection
        tft.print("Pinging MQTT server ");
        tft.print(mqttServer);
        tft.println("...");
        
        bool pingSuccess = Ping.ping(mqttServer, 3);
        
        if (pingSuccess) {
            tft.println("Ping successful!");
            tft.print("Avg time: ");
            tft.print(Ping.averageTime());
            tft.println("ms");
        } else {
            tft.println("Ping failed!");
            tft.println("MQTT server unreachable");
            return false;
        }
        
        tft.println("Connecting to MQTT...");
        String clientId = "ESP32Client-" + String(random(0xffff), HEX);
        
        if (mqttClient.connect(clientId.c_str())) {
            tft.println("MQTT connected!");
            return true;
        } else {
            tft.print("MQTT failed, rc=");
            tft.println(mqttClient.state());
            return false;
        }
    }
    return true;
}

void setup() {
    Serial.begin(112500);
    setupDisplay();
    setupWifi();
    setupMqtt();
    connectMqtt();
}

void showDiamondColors();

void loop() {
    tft.setCursor(10, 50);
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    
    if (WiFi.status() != WL_CONNECTED) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0,0);
        tft.println("WiFi Disconnected!");
        setupWifi();
    }
    else {
        tft.println(WiFi.localIP());
        tft.print("My IP: ");
    }
    
    // Ensure MQTT connection
    if (!mqttClient.connected()) {
        connectMqtt();
    }
    else {
        mqttClient.publish("hello", "hello world");
        mqttClient.loop();
    }
    
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
