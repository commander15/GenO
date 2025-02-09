#include "object.h"
#include "object_p.h"

#include <GenO/private/debug_p.h>
#include <GenO/serialization.h>

#include <QtCore/qvariant.h>
#include <QtCore/qjsonobject.h>

namespace GenO {

Object::Object()
    : Object(new ObjectPrivate)
{
}

Object::Object(const Object &other)
    : d_ptr(other.d_ptr)
{
    sync(*this, other);
}

Object::Object(Object &&other)
    : d_ptr(other.d_ptr)
{
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
    return property(name, QVariant());
}

QVariant Object::property(const QString &name, const QVariant &defaultValue) const
{
    if (name.isEmpty())
        return defaultValue;

    const QStringList path = name.split('.', Qt::SkipEmptyParts);

    if (path.size() == 1)
        return (hasProperty(path.first()) ? readProperty(path.first()) : defaultValue);

    QVariant value = readProperty(path.first());
    auto object = reinterpret_cast<Object*>(value.data()); // Cast from void*

    for (int i = 1; i < path.size() - 1; ++i) {
        if (!object || !object->hasProperty(path.at(i)))
            return defaultValue;

        value = object->readProperty(path.at(i));
        object = reinterpret_cast<Object*>(value.data()); // Reinterpret again
    }

    return (object && object->hasProperty(path.last())) ? object->readProperty(path.last()) : defaultValue;
}

void Object::setProperty(const QString &name, const QVariant &value)
{
    if (name.isEmpty())
        return;

    const QStringList path = name.split('.', Qt::SkipEmptyParts);

    if (path.size() == 1) {
        // Direct write to the root property
        writeProperty(path.first(), value);
        return;
    }

    QVariant currentValue = readProperty(path.first());
    auto rootObject = reinterpret_cast<Object*>(currentValue.data()); // Start from the root
    if (!rootObject)
        return;

    QVariant nestedValue = currentValue; // Keep track of the current nested value
    auto currentObject = rootObject;

    for (int i = 1; i < path.size() - 1; ++i) {
        if (!currentObject->hasProperty(path.at(i)))
            return;

        nestedValue = currentObject->readProperty(path.at(i));
        currentObject = reinterpret_cast<Object*>(nestedValue.data());
        if (!currentObject)
            return;
    }

    // Set the value on the target nested object
    if (currentObject && currentObject->hasProperty(path.last())) {
        currentObject->writeProperty(path.last(), value);
    }

    // Propagate the changes back up to the root
    for (int i = path.size() - 2; i >= 0; --i) {
        QVariant updatedValue = QVariant::fromMetaType(currentObject->instanceMetaObject()->metaType(), currentObject); // Wrap the updated object
        currentObject = reinterpret_cast<Object*>(nestedValue.data());

        if (!currentObject)
            return;

        currentObject->writeProperty(path.at(i), updatedValue); // Update parent object
        nestedValue = QVariant::fromMetaType(currentObject->instanceMetaObject()->metaType(), currentObject);
    }

    // Finally, update the root object
    writeProperty(path.first(), nestedValue);
}

QStringList Object::propertyNames() const
{
    QStringList properties;

    const QMetaObject *meta = instanceMetaObject();
    for (int i(0); i < meta->propertyCount(); ++i)
        properties.append(meta->property(i).name());

    properties.append(d_ptr->propertyNames());

    return properties;
}

bool Object::boolean(const QString &property) const
{
    return this->property(property).toBool();
}

int Object::integer(const QString &property) const
{
    return this->property(property).toInt();
}

double Object::number(const QString &property) const
{
    return this->property(property).toDouble();
}

QString Object::string(const QString &property) const
{
    return this->property(property).toString();
}

bool Object::isSubObject(const QString &propertyName) const
{
    const QVariant value = readProperty(propertyName);
    const QMetaType type = value.metaType();
    const QMetaObject *object = type.metaObject();
    return (object ? object->inherits(&staticMetaObject) : false);
}

QMetaType Object::subObjectType(const QString &property) const
{
    const QVariant value = readProperty(property);
    return value.metaType();
}

bool Object::isList(const QString &propertyName) const
{
    const QVariant value = readProperty(propertyName);
    const QMetaType type = value.metaType();
    return QString::fromLatin1(type.name()).startsWith("QList");
}

QMetaType Object::listItemType(const QString &propertyName) const
{
    const QMetaObject *meta = metaObject();
    const int propertyIndex = meta->indexOfProperty(propertyName.toStdString().c_str());
    const QString typeName = meta->property(propertyIndex).typeName();

    if (typeName.startsWith("QList<") && typeName.endsWith(">"))
        return QMetaType::fromName(typeName.mid(6, typeName.length() - 7).toLatin1());
    else if (typeName == "QStringList")
        return QMetaType::fromType<QString>();
    else if (typeName == "QByteArrayList")
        return QMetaType::fromType<QByteArray>();
    else
        return QMetaType();
}

bool Object::isValid() const
{
    return d_ptr->isValid();
}

bool Object::isEmpty() const
{
    return d_ptr->isEmpty();
}

Object Object::fromJson(const QJsonObject &json)
{
    Object object;
    Serialization::load(json, &object);
    return object;
}

QJsonObject Object::toJsonObject() const
{
    QJsonObject object;
    Serialization::save(object, this);
    return object;
}

const QMetaObject *Object::metaObject() const
{
    init();
    return d_ptr->metaObject();
}

const QMetaObject *Object::instanceMetaObject() const
{
    return &staticMetaObject;
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
    const QMetaObject *meta1 = o.instanceMetaObject();
    const QMetaObject *meta2 = o.metaObject();
    return sameMetaObject(meta1, meta2);
}

bool Object::sameType(const Object &o1, const Object &o2)
{
    const QMetaObject *meta1 = o1.instanceMetaObject();
    const QMetaObject *meta2 = o2.instanceMetaObject();
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
    ObjectData *d = o1.d_ptr;
    d->storeProperties(&o2);
    d->restoreProperties(&o1);
}

void Object::init() const
{
    auto d = const_cast<ObjectData *>(d_ptr.data());
    if (d->metaObject())
        return;

    registerType();
    d->init(instanceMetaObject());
}

void Object::registerType() const
{
}

QVariant Object::readProperty(const QString &name) const
{
    init();
    const QMetaObject *meta = instanceMetaObject();

    int index = (meta ? meta->indexOfProperty(name.toStdString().c_str()) : -1);
    if (index >= 0)
        return meta->property(index).readOnGadget(this);

    if (d_ptr->hasProperty(name)) {
        return d_ptr->readProperty(name);
    }

    return QVariant();
}

bool Object::writeProperty(const QString &name, const QVariant &value)
{
    init();
    const QMetaObject *meta = instanceMetaObject();

    int index = (meta ? meta->indexOfProperty(name.toStdString().c_str()) : -1);
    if (index >= 0) {
        if (meta->property(index).writeOnGadget(this, value))
            return true;
        else {
            genoWarning() << "can't write property '" << name << "' on " << meta->className();
            return false;
        }
    } else
        return d_ptr->writeProperty(name, value);
}

void ObjectData::init(const QMetaObject *meta)
{
    if (m_metaObject)
        return;

    m_metaObject = meta;
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

bool sameMetaObject(const QMetaObject *o1, const QMetaObject *o2)
{
    return o1 == o2;
}

} // namespace GenO

template<>
GenO::ObjectData *QSharedDataPointer<GenO::ObjectData>::clone()
{
    GenO::ObjectData *c = d->clone();
    return (c ? c : d);
}
