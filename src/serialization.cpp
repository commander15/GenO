#include "serialization.h"

#include <GenO/object.h>
#include <GenO/jsonserialization.h>

namespace GenO {

void Serialization::load(const QVariant &source, Object *object, int format)
{
    load(source.data(), object, format);
}

void Serialization::save(QVariant &source, const Object *object, int format)
{
    save(source.data(), object, format);
}

void Serialization::load(const void *source, Object *object, int format)
{
    if (source == nullptr)
        return;

    switch (format) {
    case Json:
        JsonSerialization::load(*static_cast<const QJsonObject *>(source), object);
        break;
    }
}

void Serialization::save(void *source, const Object *object, int format)
{
    if (source == nullptr)
        return;

    switch (format) {
    case Json:
        JsonSerialization::save(*static_cast<QJsonObject *>(source), object);
        break;
    }
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
