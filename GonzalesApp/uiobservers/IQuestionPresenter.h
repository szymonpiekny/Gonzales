#pragma once

#include "../qa/Question.h"

class IQuestionPresenter
{
public:
    virtual void presentQuestion(const Question &question) = 0;
    virtual void clear() = 0;
    virtual ~IQuestionPresenter(){}
};
