#pragma once

enum statusEnum {FINDQUIZ, GETQUESTION, SUBMITRESPONSE, GETCORRECTION};

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