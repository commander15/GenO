#ifndef GENO_OBJECT_H
#define GENO_OBJECT_H

#include <GenO/global.h>
#include <GenO/objectinterface.h>
#include <GenO/serialization.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qmetaobject.h>

#define G_OBJECT(ClassName)   G_OBJECT_IMPL(ClassName) G_METHODS_IMPL(ClassName)
#define G_DATA(ClassName)     G_OBJECT_IMPL(ClassName)

#define G_RESOURCE(Endpoint)  Q_CLASSINFO("G.Endpoint", Endpoint)
#define G_ID(ID)              Q_CLASSINFO("G.Id", ID)

#define G_METHODS(ClassName)  G_METHODS_IMPL(ClassName)
#define G_REGISTER(ClassName) G_REGISTER_IMPL(ClassName)

#define G_OBJECT_IMPL(ClassName) \
public: \
    const QMetaObject *instanceMetaObject() const override \
    { return &ClassName::staticMetaObject; } \
    static void registerClass() { \
        qRegisterMetaType<ClassName>(); \
        if (QMetaType::hasRegisteredConverterFunction<QVariantList, QList<ClassName>>()) \
            return; \
        QMetaType::registerConverter<QVariantList, QList<ClassName>>([](const QVariantList &list) { \
            QList<ClassName> result; \
            for (const QVariant &item : list) \
                if (item.canConvert<ClassName>()) \
                    result.append(item.value<ClassName>()); \
            return result; \
        }); \
    } \
protected: \
    void registerType() const override \
    { ClassName::registerClass(); }

#define G_METHODS_IMPL(ClassName) \
public: \
    static ClassName fromJson(const QJsonObject &json) \
    { return GenO::Object::fromJson<ClassName>(json); }

#define G_REGISTER_IMPL(ClassName) \
class ClassName##Registrator : public GenO::ObjectRegistrator \
{ \
public: \
    ClassName##Registrator() \
    { registerType(); } \
    void registerType() override {\
        ClassName::registerClass(); \
        qDebug() << #ClassName << "class registered !"; \
    } \
}; \
static ClassName##Registrator ClassName##Reg;


namespace GenO {

class ObjectData;
class GENO_EXPORT Object : public ObjectInterface
{
    Q_GADGET

public:
    Object();
    template<typename Data> Object() : d_ptr(new Data()) {}
    Object(const Object &other);
    Object(Object &&other);

    Object &operator=(const Object &other);
    Object &operator=(Object &&other);

    bool hasProperty(const QString &name) const override;
    QVariant property(const QString &name) const;
    QVariant property(const QString &name, const QVariant &defaultValue) const;
    void setProperty(const QString &name, const QVariant &value);
    QStringList propertyNames() const override;

    bool boolean(const QString &property) const;
    int integer(const QString &property) const;
    double number(const QString &property) const;
    QString string(const QString &property) const;

    bool isSubObject(const QString &propertyName) const;
    QMetaType subObjectType(const QString &property) const;

    bool isList(const QString &propertyName) const;
    QMetaType listItemType(const QString &propertyName) const;

    bool isValid() const override;
    bool isEmpty() const override;

    static Object fromJson(const QJsonObject &json);
    template<typename T> static T fromJson(const QJsonObject &json);
    QJsonObject toJsonObject() const;

    template<typename T>
    T to() const;

    const QMetaObject *metaObject() const override;
    virtual const QMetaObject *instanceMetaObject() const;

    bool operator==(const Object &other) const;
    bool operator!=(const Object &other) const;

    static bool original(const Object &o);
    static bool sameType(const Object &o1, const Object &o2);
    static void transferProperties(const Object &o1, Object &o2);
    static void sync(Object &o1, const Object &o2);

protected:
    Object(ObjectData *data);
    Object(const QSharedDataPointer<ObjectData> &data);

    void init() const;
    virtual void registerType() const;

    QVariant readProperty(const QString &name) const override final;
    bool writeProperty(const QString &name, const QVariant &value) override final;

    QSharedDataPointer<ObjectData> d_ptr;

    friend class ObjectData;
    friend class Serialization;
};

class GENO_EXPORT ObjectData : public QSharedData, public ObjectInterface
{
public:
    ObjectData() = default;
    ObjectData(const ObjectData &other) = default;

    void init(const QMetaObject *meta);

    bool hasProperty(const QString &name) const override;
    QVariant readProperty(const QString &name) const override;
    bool writeProperty(const QString &name, const QVariant &value) override;
    QStringList propertyNames() const override;

    void restoreProperties(Object *object);
    void storeProperties(const Object *object);

    bool isValid() const override { return true; }
    bool isEmpty() const override { return properties.isEmpty(); }

    inline const QMetaObject *metaObject() const override
    { return m_metaObject; }

    virtual ObjectData *clone() const
    { return nullptr; }

    QHash<QString, QString> attributes;
    QMap<QString, QVariant> properties;

private:
    const QMetaObject *m_metaObject = nullptr;

    friend class Object;
};

class GENO_EXPORT ObjectRegistrator
{
public:
    virtual void registerType() = 0;
};

template<typename T>
inline T Object::to() const
{
    T object;
    sync(object, *this);
    return object;
}

template<typename T>
inline T Object::fromJson(const QJsonObject &json)
{
    T object;
    Serialization::load(json, &object);
    return object;
}

} // namespace GenO

Q_DECLARE_METATYPE(GenO::Object)

#endif // GENO_OBJECT_H
