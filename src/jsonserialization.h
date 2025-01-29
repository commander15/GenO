#ifndef JSONSERIALIZATION_H
#define JSONSERIALIZATION_H

#include <GenO/global.h>
#include <GenO/serialization.h>

class QJsonObject;

namespace GenO {

class Object;

class GENO_EXPORT JsonSerialization : public Serialization
{
public:
    static bool support(const QMetaType &type);

    static void load(const QVariant &value, Object *object);
    static void save(QVariant &value, const Object *object);

    static void load(const QJsonObject &json, Object *object);
    static void save(QJsonObject &json, const Object *object);
};

} // namespace GenO

GENO_EXPORT const QJsonObject &operator<<(const QJsonObject &json, GenO::Object &object);
GENO_EXPORT QJsonObject &operator>>(QJsonObject &json, const GenO::Object &object);

#endif // JSONSERIALIZATION_H
