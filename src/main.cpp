#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
//this file should define WIFI_SSID and WIFI_PWD, MQTT_USER and MQTT_PWD,
//it is not included in the repository for security reasons
#include "password.h"
#include <PubSubClient.h>

#include "Input.hpp"
#include "quiz.hpp"

// see incoming messages with command:
// mosquitto_sub -h localhost -t Quiz/# -u bart -P bart
// simulate quiz master with:
// mosquitto_pub -h localhost -u bart -P bart -t Quiz/Announce -m "HelloWorld"

TFT_eSPI    tft = TFT_eSPI();
TFT_eSprite square = TFT_eSprite(&tft);
IPAddress mqttServer(192,168,0,156);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Input input;
Participant participant(tft, input, mqttClient);
statusEnum status(statusEnum::FINDQUIZ);

void setupDisplay() {
    tft.setRotation(4); //setRotation also sets some flags inside the AdaFruit drivers. After that, we manually set MADCTL
    tft.init(TFT_BLACK);
    tft.writecommand(TFT_MADCTL);
    tft.writedata(TFT_MAD_BGR | TFT_MAD_MV); //exchange red and blue bytes, and mirror x-coordinates
    tft.fillScreen(TFT_BLACK);

    square.createSprite(50, 50);
    square.fillSprite(TFT_BLACK);
    square.fillRect(0, 0, 20, 20, TFT_RED);
    square.fillRect(30, 0, 20, 20, TFT_GREEN);
    square.fillRect(0, 30, 20, 20, TFT_BLUE);
    square.fillRect(30, 30, 20, 20, TFT_YELLOW);
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

void MqttToParticipantCallback (char* topic, byte* payload, unsigned int length) {
    participant.mqttCallback(topic, payload, length);
}

bool connectMqtt() {
    tft.println("Connecting to MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PWD)) {
        tft.println("MQTT connected!");
        mqttClient.setCallback(MqttToParticipantCallback);
        return true;
    } else {
        tft.print("MQTT failed, rc=");
        tft.println(mqttClient.state());
        return false;
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
    
    switch (status)
    {
    case statusEnum::FINDQUIZ:
        status = participant.FindQuiz();
        break;
    case statusEnum::GETQUESTION:
        status = participant.GetQuestion();
        break;
    case statusEnum::SUBMITRESPONSE:
        status = participant.SubmitResponse();
        break;
    case statusEnum::GETCORRECTION:
        status = participant.GetCorrection();
        break;
    default:
        tft.println("Unknown status");
        delay(2000);
        break;
    }
    // Ensure MQTT connection
    if (!mqttClient.connected()) {
        connectMqtt();
    }
    mqttClient.loop();

    
    showDiamondColors();
    delay(2000);
}

void showDiamondColors() {
    tft.fillScreen(TFT_BLACK);
    tft.setPivot(200, 150);
    square.pushRotated(45);
    tft.setTextColor(TFT_BLACK, TFT_RED);
    tft.setCursor(200 - 5, 150 - 25);
    tft.println("X");
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.setCursor(200 + 17, 150 - 10);
    tft.println("A");
    tft.setTextColor(TFT_BLACK, TFT_BLUE);
    tft.setCursor(200 - 23, 150 - 10);
    tft.println("Y");
    tft.setTextColor(TFT_BLACK, TFT_YELLOW);
    tft.setCursor(200 - 5, 150 + 15);
    tft.println("B");
}
