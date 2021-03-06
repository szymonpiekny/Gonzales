#pragma once

#include "DataDeserializer.h"
#include "File.h"

class IDataDeserializerFactory
{
public:
    virtual std::shared_ptr<IDataDeserializer> create(QIODevice *device) const=0;
    virtual ~IDataDeserializerFactory(){}
};


class DataDeserializerFactory : public IDataDeserializerFactory
{
public:
    std::shared_ptr<IDataDeserializer> create(QIODevice *device) const
    {
        return std::shared_ptr<IDataDeserializer>(new DataDeserializer(device));
    }
};
