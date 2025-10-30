#include <Arduino.h>
#include <TFT_eSPI.h>
#include <PubSubClient.h>

#include "quiz.hpp"


Participant::Participant(TFT_eSPI& tft, PubSubClient& mqttClient) : tft(tft), mqttClient(mqttClient)
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
    tft.println("Finding Quiz...");
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