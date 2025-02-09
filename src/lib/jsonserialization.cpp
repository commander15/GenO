#include "jsonserialization.h"

#include <GenO/object.h>

#include <GenO/private/debug_p.h>

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonvalue.h>
#include <QtCore/qvariant.h>

namespace GenO {

bool JsonSerialization::support(const QMetaType &type)
{
    return type == QMetaType::fromType<QJsonObject>() || type == QMetaType::fromType<QJsonValue>();
}

void JsonSerialization::load(const QVariant &value, Object *object)
{
    const QMetaType type = value.metaType();

    if (type == QMetaType::fromType<QJsonObject>())
        load(value.toJsonObject(), object);
    else if (type == QMetaType::fromType<QJsonValue>())
        load(value.toJsonValue().toObject(), object);
}

void JsonSerialization::save(QVariant &value, const Object *object)
{
    const QMetaType type = value.metaType();

    if (type == QMetaType::fromType<QJsonObject>()) {
        save(*static_cast<QJsonObject *>(value.data()), object);
    } else if (type == QMetaType::fromType<QJsonValue>()) {
        QJsonObject o = value.toJsonValue().toObject();
        save(o, object);
        value = QJsonValue(o);
    }
}

void JsonSerialization::load(const QJsonObject &json, Object *object)
{
    const QStringList fields = json.keys();
    for (const QString &field : fields) {
        const QString property = propertyName(field);
        const QJsonValue value = json.value(field);

        if (value.isObject()) {
            QMetaType type = object->subObjectType(property);
            if (!type.isValid()) {
                genoWarning() << "Unknown object type for property '" << property
                              << "' on " << object->metaObject()->className();
                continue;
            }

            QVariant val = readProperty(property, object);
            if (val.isValid()) {
                Object *o = static_cast<Object *>(val.data());
                load(value.toObject(), o);
                writeProperty(property, val, object);
            }
        } else if (value.isArray()) {
            QMetaType type = object->listItemType(property);
            if (!type.isValid()) {
                genoWarning() << "Unknown list item type for property '" << property
                              << "' on " << object->metaObject()->className();
                continue;
            }

            const QMetaObject *meta = type.metaObject();

            bool isGObject = (meta ? meta->inherits(&Object::staticMetaObject) : false);

            QVariantList items;

            QJsonArray itemArray = value.toArray();
            for (const QJsonValue &itemValue : itemArray) {
                if (isGObject) {
                    Object *o = static_cast<Object *>(type.create());
                    load(itemValue.toObject(), o);
                    items.append(QVariant::fromMetaType(type, o));
                    type.destroy(o);
                } else {
                    items.append(itemValue.toVariant());
                }
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
        const QString field = fieldName(property);

        if (object->isSubObject(property)) {
            QJsonObject subJson;

            QVariant subValue = readProperty(property, object);
            if (subValue.metaType().metaObject()->inherits(&Object::staticMetaObject))
                save(subJson, static_cast<Object *>(subValue.data()));

            json.insert(field, subJson);
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
            json.insert(field, items);
        } else {
            json.insert(field, QJsonValue::fromVariant(readProperty(property, object)));
        }
    }
}

#include <QString>
#include <QRegularExpression>

QString JsonSerialization::propertyName(const QString &fieldName)
{
    QStringList parts = fieldName.split('_', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return QString();

    QString camelCase = parts[0].toLower(); // Keep the first part lowercase
    for (int i = 1; i < parts.size(); ++i)
        camelCase += parts[i].left(1).toUpper() + parts[i].mid(1).toLower();

    return camelCase;
}

QString JsonSerialization::fieldName(const QString &propertyName)
{
    QRegularExpression regex("([a-z])([A-Z])");
    QString snake_case = propertyName;
    snake_case.replace(regex, "\\1_\\2"); // Insert underscore before uppercase letters
    return snake_case.toLower();
}

} // namespace GenO

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
