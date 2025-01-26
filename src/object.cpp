#include "object.h"
#include "object_p.h"

#include <QtCore/qvariant.h>

namespace GenO {

Object::Object()
    : Object(new ObjectPrivate)
{
}

Object::Object(const Object &other)
    : d_ptr(other.d_ptr)
{
    if (!sameType(*this, other))
        sync(*this, other);
}

Object::Object(Object &&other)
    : d_ptr(std::move(other.d_ptr))
{
    if (!sameType(*this, other))
        sync(*this, other);
}

Object::Object(ObjectData *data)
    : d_ptr(data)
{
}

Object::Object(const QSharedDataPointer<ObjectData> &data)
    : d_ptr(data)
{
}

Object &Object::operator=(const Object &other)
{
    if (this != &other) {
        d_ptr = other.d_ptr;

        if (!sameType(*this, other))
            sync(*this, other);
    }
    return *this;
}

Object &Object::operator=(Object &&other)
{
    if (this != &other) {
        d_ptr.swap(other.d_ptr);

        if (!sameType(*this, other))
            sync(*this, other);
    }
    return *this;
}

bool Object::hasProperty(const QString &name) const
{
    return readProperty(name).isValid();
}

QVariant Object::property(const QString &name) const
{
    return readProperty(name);
}

QVariant Object::property(const QString &name, const QVariant &defaultValue) const
{
    return (hasProperty(name) ? readProperty(name) : defaultValue);
}

void Object::setProperty(const QString &name, const QVariant &value)
{
    writeProperty(name, value);
}

QStringList Object::propertyNames() const
{
    QStringList properties;

    const QMetaObject meta = instanceMetaObject();
    for (int i(0); i < meta.propertyCount(); ++i)
        properties.append(meta.property(i).name());

    properties.append(d_ptr->propertyNames());

    return properties;
}

bool Object::isValid() const
{
    return d_ptr->isValid();
}

bool Object::isEmpty() const
{
    return d_ptr->isEmpty();
}

const QMetaObject Object::metaObject() const
{
    init();
    return d_ptr->metaObject();
}

const QMetaObject Object::instanceMetaObject() const
{
    return staticMetaObject;
}

bool Object::operator==(const Object &other) const
{
    if (d_ptr == other.d_ptr) {
        return true;
    } else {
        return sameType(*this, other) && d_ptr->properties == other.d_ptr->properties;
    }
}

bool Object::operator!=(const Object &other) const
{
    return !operator==(other);
}

bool Object::original(const Object &o)
{
    const QMetaObject meta1 = o.instanceMetaObject();
    const QMetaObject meta2 = o.metaObject();
    return sameMetaObject(meta1, meta2);
}

bool Object::sameType(const Object &o1, const Object &o2)
{
    const QMetaObject meta1 = o1.instanceMetaObject();
    const QMetaObject meta2 = o2.instanceMetaObject();
    return sameMetaObject(meta1, meta2);
}

void Object::transferProperties(const Object &o1, Object &o2)
{
    const QStringList properties = o1.propertyNames();
    for (const QString &property : properties)
        o2.writeProperty(property, o1.readProperty(property));
}

void Object::sync(Object &o1, const Object &o2)
{
    o1.d_ptr->storeProperties(&o2);
    o1.d_ptr->restoreProperties(&o1);
}

void Object::init() const
{
    auto d = const_cast<ObjectData *>(d_ptr.data());
    d->init(instanceMetaObject());
}

QVariant Object::readProperty(const QString &name) const
{
    const QMetaObject meta = instanceMetaObject();

    int index = meta.indexOfProperty(name.toStdString().c_str());
    if (index >= 0)
        return meta.property(index).readOnGadget(this);

    if (d_ptr->hasProperty(name)) {
        return d_ptr->readProperty(name);
    }

    return QVariant();
}

bool Object::writeProperty(const QString &name, const QVariant &value)
{
    const QMetaObject meta = instanceMetaObject();

    int index = meta.indexOfProperty(name.toStdString().c_str());
    if (index >= 0)
        return meta.property(index).writeOnGadget(this, value);

    if (d_ptr->hasProperty(name)) {
        return d_ptr->writeProperty(name, value);
    }

    return false;
}

void ObjectData::init(const QMetaObject &meta)
{
    if (m_initialized)
        return;

    m_metaObject = meta;
    m_initialized = true;
}

bool ObjectData::hasProperty(const QString &name) const
{
    return properties.contains(name);
}

QVariant ObjectData::readProperty(const QString &name) const
{
    return properties.value(name);
}

bool ObjectData::writeProperty(const QString &name, const QVariant &value)
{
    properties.insert(name, value);
    return true;
}

QStringList ObjectData::propertyNames() const
{
    return properties.keys();
}

void ObjectData::restoreProperties(Object *object)
{
    const QStringList properties = object->propertyNames();
    for (const QString &property : properties)
        if (this->properties.contains(property))
            object->writeProperty(property, this->properties.take(property));
}

void ObjectData::storeProperties(const Object *object)
{
    const QStringList properties = object->propertyNames();
    for (const QString &property : properties)
        this->properties.insert(property, object->readProperty(property));
}

ObjectData *ObjectPrivate::clone() const
{
    return new ObjectPrivate(*this);
}

bool sameMetaObject(const QMetaObject &o1, const QMetaObject &o2)
{
    return &o1.d == &o2.d;
}

} // namespace GenO

template<>
GenO::ObjectData *QSharedDataPointer<GenO::ObjectData>::clone()
{
    GenO::ObjectData *c = d->clone();
    return (c ? c : d);
}
