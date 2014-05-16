#pragma once
#include "../Common/SimpleTree/Node.h"
#include "qacontainer.h"

class IQAsAppender
{
public:
    virtual void append(std::vector<std::shared_ptr<QA> > importedQAs) = 0;
    virtual ~IQAsAppender(){}
};
