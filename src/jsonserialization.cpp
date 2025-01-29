#include "jsonserialization.h"

#include <GenO/object.h>

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonvalue.h>
#include <QtCore/qvariant.h>

namespace GenO {

void JsonSerialization::load(const QJsonObject &json, Object *object)
{
    const QStringList properties = json.keys();
    for (const QString &property : properties) {
        const QJsonValue value = json.value(property);
        if (value.isObject()) {
            QVariant val = readProperty(property, object);
            if (val.isValid()) {
                Object *o = static_cast<Object *>(val.data());
                load(value.toObject(), o);
                writeProperty(property, val, object);
            }
        } else if (value.isArray()) {
            QMetaType type = object->listItemType(property);
            QVariantList items;

            QJsonArray itemArray = value.toArray();
            for (const QJsonValue &itemValue : itemArray) {
                Object *o = static_cast<Object *>(type.create());
                load(itemValue.toObject(), o);
                items.append(QVariant::fromMetaType(type, o));
                type.destroy(o);
            }

            writeProperty(property, items, object);
        } else {
            writeProperty(property, value.toVariant(), object);
        }
    }
}

void JsonSerialization::save(QJsonObject &json, const Object *object)
{
    const QStringList properties = object->propertyNames();
    for (const QString &property : properties) {
        if (object->isSubObject(property)) {
            QJsonObject subJson;

            QVariant subValue = readProperty(property, object);
            if (subValue.metaType().metaObject()->inherits(&Object::staticMetaObject))
                save(subJson, static_cast<Object *>(subValue.data()));

            json.insert(property, subJson);
        } else if (object->isList(property)) {
            QJsonArray items;
            QVariantList values = readProperty(property, object).toList();
            for (QVariant &value : values) {
                QJsonObject item;
                if (value.metaType().metaObject()->inherits(&Object::staticMetaObject)) {
                    save(item, static_cast<Object *>(value.data()));
                    items.append(item);
                }
            }
            json.insert(property, items);
        } else {
            json.insert(property, QJsonValue::fromVariant(readProperty(property, object)));
        }
    }
}

} // namespace GenO

GenO::Object &operator<<(GenO::Object &object, const QJsonObject &json)
{
    GenO::JsonSerialization::load(json, &object);
    return object;
}

const GenO::Object &operator>>(const GenO::Object &object, QJsonObject &json)
{
    GenO::JsonSerialization::save(json, &object);
    return object;
}

const QJsonObject &operator<<(const QJsonObject &json, GenO::Object &object)
{
    GenO::JsonSerialization::load(json, &object);
    return json;
}

QJsonObject &operator>>(QJsonObject &json, const GenO::Object &object)
{
    GenO::JsonSerialization::save(json, &object);
    return json;
}
