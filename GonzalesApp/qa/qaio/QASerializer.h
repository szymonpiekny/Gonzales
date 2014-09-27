#pragma once
#include "../../common/SimpleTree/NodeSerializer.h"
#include "../../common/qtproxies/DataSerializer.h"
#include "../../common/qtproxies/DataDeserializer.h"
#include "IQASerializer.h"

class IQADeserializer
{
public:
    virtual void deserialize(IDataDeserializer &deserializer, SimpleTree::Node<QA> &node) = 0;
    virtual ~IQADeserializer(){}
};

class QASerializer: public IQASerializer, public IQADeserializer
{
public:
    QASerializer():
        serializer{std::make_shared<SimpleTree::Utils::InfosSerializer>()}
    {}
    void serialize(IDataSerializer& DataSerializer, const SimpleTree::Node<QA> &node)
    {
        serializer.serialize(DataSerializer, node);
    }

    void deserialize(IDataDeserializer &DataDeserializer, SimpleTree::Node<QA> &node)
    {
        serializer.deserialize(DataDeserializer, node);
    }

private:
    SimpleTree::Utils::NodeSerializer serializer;
};