#ifndef GENO_SERIALIZATION_H
#define GENO_SERIALIZATION_H

#include <GenO/global.h>

class QJsonObject;

namespace GenO {

class Object;

class GENO_EXPORT Serialization
{
public:
    static void load(const QVariant &source, Object *object);
    static void save(QVariant &source, const Object *object);

    static void load(const QJsonObject &json, Object *object);
    static void save(QJsonObject &json, const Object *object);

protected:
    static QVariant readProperty(const QString &name, const Object *object);
    static bool writeProperty(const QString &name, const QVariant &value, Object *object);
};

} // namespace GenO

GENO_EXPORT QDebug operator<<(QDebug debug, GenO::Object &object);

#endif // GENO_SERIALIZATION_H
