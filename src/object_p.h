#ifndef GENO_OBJECT_P_H
#define GENO_OBJECT_P_H

#include "object.h"

namespace GenO {

class ObjectPrivate : public ObjectData
{
public:
    ObjectData *clone() const override;
};

static bool sameMetaObject(const QMetaObject &o1, const QMetaObject &o2);

} // namespace GenO

#endif // GENO_OBJECT_P_H
