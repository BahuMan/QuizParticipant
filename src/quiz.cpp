#include <Arduino.h>
#include <TFT_eSPI.h>
#include <PubSubClient.h>

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
    delay(2000);
    return GETQUESTION;
}

/**
 * wait for a question to be published in the queue of the chosen quiz
 */
statusEnum Participant::GetQuestion()
{
    tft.println("Getting Question...");
    delay(2000);
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

    // Check topic and process message
    if (strcmp(topic, "Quiz/Announce") == 0) {
        memcpy(payload, this->quizQueue, length);
        this->quizQueue[length] = '\0';
    }
    else {
        tft.println("Unknown topic");
        tft.println((char*)payload);
    }
}
