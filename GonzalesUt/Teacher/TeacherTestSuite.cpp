#include <iostream>
#include "../gtest.h"
#include <../../GonzalesApp/teaching/teacher.h>
#include <utility>
#include <memory>
#include <exception>
#include "TextPresenterMock.h"
#include "QasProviderMock.h"


class TeacherTestSuite: public ::testing::Test
{

protected:
    TeacherTestSuite():
        questionPresenterMock(std::make_shared<TextPresenterMock>()),
        answerPresenterMock(std::make_shared<TextPresenterMock>()),
        m_qasProvider(std::make_shared<QasProviderMock>()),
        teacher(questionPresenterMock, answerPresenterMock, m_qasProvider)
    {

    }
    void SetUp()
    {
        oneQuestionQueue.push_back(QA(Question("question1"), Answer("answer1")));

        twoQuestionQueue.push_back(QA(Question("question1"), Answer("answer1")));
        twoQuestionQueue.push_back(QA(Question("question2"), Answer("answer2")));

        sixQuestionQueue.push_back(QA(Question("question1"), Answer("answer1")));
        sixQuestionQueue.push_back(QA(Question("question2"), Answer("answer2")));
        sixQuestionQueue.push_back(QA(Question("question3"), Answer("answer3")));
        sixQuestionQueue.push_back(QA(Question("question4"), Answer("answer4")));
        sixQuestionQueue.push_back(QA(Question("question5"), Answer("answer5")));
        sixQuestionQueue.push_back(QA(Question("question6"), Answer("answer6")));
    }

    void checkNextQuestion(const std::string & expectedQuestionString)
    {
        teacher.showNextQuestion();
        //EXPECT_EQ(questionFromTeacher.getAsString(), expectedQuestionString);
    }

    void checkAnswerForNextQuestionIsCorrect(std::string answer)
    {
        teacher.showNextQuestion();
        ASSERT_TRUE(isAnswerCorrect(answer));
    }

    void checkAnswerForNextQuestionIsInCorrect(std::string answer)
    {
        teacher.showNextQuestion();
        ASSERT_FALSE(isAnswerCorrect(answer));
    }

    bool isAnswerCorrect(std::string answer)
    {
        return false;//teacher.checkAnswer(Answer(answer));
    }

    void setOneQuestion()
    {
        teacher.setQuestions(oneQuestionQueue);
    }

    void setTwoQuestions()
    {
        teacher.setQuestions(twoQuestionQueue);
    }

    void setSixQuestions()
    {
        teacher.setQuestions(sixQuestionQueue);
    }

    std::shared_ptr<TextPresenterMock> questionPresenterMock;
    std::shared_ptr<TextPresenterMock> answerPresenterMock;
    std::shared_ptr<QasProviderMock> m_qasProvider;
    Teacher teacher;
    std::vector<QA> oneQuestionQueue;
    std::vector<QA> twoQuestionQueue;
    std::vector<QA> sixQuestionQueue;

};

TEST_F(TeacherTestSuite, ZeroQuestionGiven_ShouldThrowNoQuestionException)
{
    ASSERT_THROW(teacher.showNextQuestion();, std::logic_error);
}

TEST_F(TeacherTestSuite, oneQuestionQueueGiven_ShouldReturnNextQuestion)
{
    setOneQuestion();
    checkNextQuestion("question1");
}

TEST_F(TeacherTestSuite, twoQuestionQueueGiven_ShouldReturnTwoNextQuestions)
{
    setTwoQuestions();
    checkNextQuestion("question1");
    checkNextQuestion("question2");
}

TEST_F(TeacherTestSuite, NoQuestionGiven_CheckAnswerShouldThrow)
{
    ASSERT_THROW(isAnswerCorrect("bad Answer"), std::logic_error);
}

TEST_F(TeacherTestSuite, oneQuestionQueueGiven_CheckAnswerShouldThrow)
{
    setOneQuestion();
    ASSERT_THROW(isAnswerCorrect("bad Answer"), std::logic_error);
}

TEST_F(TeacherTestSuite, oneQuestionGivenOneWrongAnswerGiven_ShouldCheckAnswerReturnFalse)
{
    setOneQuestion();
    checkAnswerForNextQuestionIsInCorrect("wrong answer");
}

TEST_F(TeacherTestSuite, oneQuestionGivenOneCorrectAnswerGiven_shouldCheckAnswerReturnTrue)
{
    setOneQuestion();
    checkAnswerForNextQuestionIsCorrect("answer1");
}


TEST_F(TeacherTestSuite, twoQuestionsGivenTwoCorrectAnswersGiven_shouldChecksAnswerReturnTrue)
{
    setTwoQuestions();
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsCorrect("answer2");
}

TEST_F(TeacherTestSuite, twoQuestionsGivenTwoCorrectAnswersGiven_shouldntBeAnyMoreToCheck)
{
    setTwoQuestions();
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsCorrect("answer2");
    ASSERT_THROW(isAnswerCorrect("dupa"), std::logic_error);
}

TEST_F(TeacherTestSuite, twoQuestionsGivenTwoCorrectAnswersGiven_shouldntBeAnyMoreToLearn)
{
    setTwoQuestions();
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsCorrect("answer2");
    ASSERT_THROW(teacher.showNextQuestion();, std::logic_error);
}

TEST_F(TeacherTestSuite, twoQuestionsGivenOneIncorrectAnswersGiven_shouldBeStillOneQuestionToLearn)
{
    setTwoQuestions();
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsInCorrect("wrong answer");
    ASSERT_NO_THROW(teacher.showNextQuestion());
}

TEST_F(TeacherTestSuite, sixQuestionsGivenThreeIncorrectAnswersGiven_shouldBeStillThreeQuestionToLearn)
{
    setSixQuestions();
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsCorrect("answer2");
    checkAnswerForNextQuestionIsCorrect("answer3");
    checkAnswerForNextQuestionIsInCorrect("answer4wrong");
    checkAnswerForNextQuestionIsInCorrect("answer5wrong");
    checkAnswerForNextQuestionIsInCorrect("answer6wrong");
    checkAnswerForNextQuestionIsCorrect("answer4");
    checkAnswerForNextQuestionIsCorrect("answer5");
    checkAnswerForNextQuestionIsCorrect("answer6");
}

TEST_F(TeacherTestSuite, NoOneQuestionGiven_GetCorrectAnswerShouldThrowNoQuestionInContainer)
{
    //ASSERT_THROW(teacher.getCorrectAnswer(Question("q")), std::logic_error);
}

TEST_F(TeacherTestSuite, OneQuestionGiven_GetCorrectAnswerShouldReturnCorrectAnswer)
{
    setOneQuestion();
    //EXPECT_EQ(Answer("answer1"), teacher.showCorrectAnswer(Question("question1")));
}

TEST_F(TeacherTestSuite, OneQuesionGiven_GetNextQuestionCalledShouldStillGetCorrectAnswer)
{
    setOneQuestion();
    teacher.showNextQuestion();
    //teacher.showCorrectAnswer(Question("question1"));
}

TEST_F(TeacherTestSuite, TwoQuestionGiven_6badAnswers2GoodAnswers)
{
    setTwoQuestions();
    checkAnswerForNextQuestionIsInCorrect("answer1-bad");
    checkAnswerForNextQuestionIsInCorrect("answer2-bad");
    checkAnswerForNextQuestionIsInCorrect("answer1-bad");
    checkAnswerForNextQuestionIsInCorrect("answer2-bad");
    checkAnswerForNextQuestionIsInCorrect("answer1-bad");
    checkAnswerForNextQuestionIsInCorrect("answer2-bad");
    checkAnswerForNextQuestionIsCorrect("answer1");
    checkAnswerForNextQuestionIsCorrect("answer2");
}

TEST_F(TeacherTestSuite, TwoQuestionGiven_GetQuestionsToLearnShouldReturnTwo)
{
    setTwoQuestions();
    EXPECT_EQ(2, teacher.questionsToLearnNum());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

