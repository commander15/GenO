#ifndef GENO_OBJECTINTERFACE_H
#define GENO_OBJECTINTERFACE_H

#include <GenO/global.h>

class QMetaObject;

namespace GenO {

class GENO_EXPORT ObjectInterface
{
public:
    virtual bool hasProperty(const QString &name) const = 0;
    virtual QVariant readProperty(const QString &name) const = 0;
    virtual bool writeProperty(const QString &name, const QVariant &value) = 0;
    virtual QStringList propertyNames() const = 0;

    virtual bool isValid() const = 0;
    virtual bool isEmpty() const = 0;

    virtual const QMetaObject metaObject() const = 0;
};

} // namespace GenO

#endif // GENO_OBJECTINTERFACE_H
