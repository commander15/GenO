#include "serialization.h"

#include <GenO/object.h>
#include <GenO/jsonserialization.h>

#include <QtCore/qjsonobject.h>

namespace GenO {

void Serialization::load(const QVariant &source, Object *object)
{
    if (JsonSerialization::support(source.metaType()))
        JsonSerialization::load(source, object);
}

void Serialization::save(QVariant &source, const Object *object)
{
    if (JsonSerialization::support(source.metaType()))
        JsonSerialization::save(source, object);
}

void Serialization::load(const QJsonObject &json, Object *object)
{
    JsonSerialization::load(json, object);
}

void Serialization::save(QJsonObject &json, const Object *object)
{
    JsonSerialization::save(json, object);
}

QVariant Serialization::readProperty(const QString &name, const Object *object)
{
    return object->readProperty(name);
}

bool Serialization::writeProperty(const QString &name, const QVariant &value, Object *object)
{
    return object->writeProperty(name, value);
}

} // namespace GenO

QDebug operator<<(QDebug debug, GenO::Object &object)
{
    QJsonObject json;
    GenO::JsonSerialization::save(json, &object);
    return debug << json;
}
