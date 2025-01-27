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
    static void load(const QJsonObject &json, Object *object);
    static void save(QJsonObject &json, const Object *object);
};

} // namespace GenO

#endif // JSONSERIALIZATION_H
