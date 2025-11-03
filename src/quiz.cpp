#include <Arduino.h>
#include <TFT_eSPI.h>
#include <PubSubClient.h>

#include "pins2024.h"
#include "Input.hpp"
#include "quiz.hpp"


Participant::Participant(TFT_eSPI& tft, Input& input, PubSubClient& mqttClient):
    tft(tft),
    input(input),
    mqttClient(mqttClient)
{

    this->choiceA[0] = '\0';
    this->choiceB[0] = '\0';
    this->choiceX[0] = '\0';
    this->choiceY[0] = '\0';
    this->question[0] = '\0';
    this->quizQueue[0] = '\0';

};

/**
 * get any quizzes published on the queue QuizAnnounce
 * and offer a choice to the user
 * when a quiz is chosen, return the next status
 * for now, we just pick the first announced quiz
 */
statusEnum Participant::FindQuiz()
{
    mqttClient.subscribe("Quiz/Announce");
    mqttClient.publish("Quiz/Participant", "Hello from participant");

    tft.println("Finding Quiz...");
    if (this->quizQueue[0] == '\0') {
        delay(2000);
        return FINDQUIZ;
    }
    this->question[0] = '\0'; //reset question
    tft.print("Chosen quiz: ");
    tft.println(this->quizQueue);
    mqttClient.unsubscribe("Quiz/Announce");
    char topic[150];
    snprintf(topic, 150, "Quiz/%s/Questions", this->quizQueue);
    mqttClient.subscribe(topic);
    snprintf(topic, 150, "Quiz/%s/Choices", this->quizQueue);
    mqttClient.subscribe(topic);
    delay(2000);
    return GETQUESTION;
}

/**
 * wait for a question to be published in the queue of the chosen quiz
 */
statusEnum Participant::GetQuestion()
{
    if (this->input.hasBeenPressed(PIN_START)) {
        return statusEnum::FINDQUIZ;
    }
    tft.println("Getting Question...");
    if (this->question[0] == '\0') {
        delay(2000);
        return GETQUESTION;
    }

    // let's agree that answers have been published before
    // the question is being published
    tft.print("Q: ");
    tft.println(this->question);
    tft.println();
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("A: "); tft.println(this->choiceA);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("B: "); tft.println(this->choiceB);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("X: "); tft.println(this->choiceX);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.print("Y: "); tft.println(this->choiceY);
    return SUBMITRESPONSE;
}   

/**
 * display question and multiple choice answers and wait for input
 */
statusEnum Participant::SubmitResponse()
{
    tft.println("Submitting Response...");
    delay(2000);
    return GETCORRECTION;
}

/**
 * once answer has been submitted, wait to see if we were right
 */
statusEnum Participant::GetCorrection()
{
    tft.println("Getting Correction...");
    delay(2000);
    return FINDQUIZ;
}

void Participant::mqttCallback(char* topic, byte* payload, unsigned int length)
{
    if (length > 99) length = 99;
    char topicBuffer[150];
    // Check topic and process message
    if (strcmp(topic, "Quiz/Announce") == 0) {
        memcpy(payload, this->quizQueue, length);
        this->quizQueue[length] = '\0';
    }
    snprintf(topicBuffer, 150, "Quiz/%s/Questions", this->quizQueue);
    if (strcmp(topic, topicBuffer) == 0) {
        memcpy(payload, this->question, length);
        this->question[length] = '\0';
    }
    snprintf(topicBuffer, 150, "Quiz/%s/Choices", this->quizQueue);
    if (strcmp(topic, topicBuffer) == 0) {
        // For simplicity, assume payload is formatted as "A:...,B:...,X:...,Y:..."
        char* token = strtok((char*)payload, ",");
        while (token != NULL) {
            if (token[0] == 'A' && token[1] == ':') {
                strncpy(this->choiceA, token + 2, 99);
            } else if (token[0] == 'B' && token[1] == ':') {
                strncpy(this->choiceB, token + 2, 99);
            } else if (token[0] == 'X' && token[1] == ':') {
                strncpy(this->choiceX, token + 2, 99);
            } else if (token[0] == 'Y' && token[1] == ':') {
                strncpy(this->choiceY, token + 2, 99);
            }   
            token = strtok(NULL, ",");
        }
    }
    else {
        tft.println("Unknown topic");
        tft.println((char*)payload);
    }
}
