#pragma once

enum statusEnum {FINDQUIZ, GETQUESTION, SUBMITRESPONSE, GETCORRECTION};

class Participant
{
public:
    Participant(TFT_eSPI& tft, PubSubClient& mqttClient);

    /**
     * get any quizzes published on the queue QuizAnnounce
     * and offer a choice to the user
     * when a quiz is chosen, return the next status
     */
    statusEnum FindQuiz();

    /**
     * wait for a question to be published in the queue of the chosen quiz
     */
    statusEnum GetQuestion();

    /**
     * display question and multiple choice answers and wait for input
     */
    statusEnum SubmitResponse();

    /**
     * once answer has been submitted, wait to see if we were right
     */
    statusEnum GetCorrection();

    private:
        TFT_eSPI& tft;
        PubSubClient& mqttClient;
        char quizQueue[100];
        char question[500];
        char choiceA[100];
        char choiceB[100];
        char choiceX[100];
        char choiceY[100];

};

