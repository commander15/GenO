#ifndef GENO_OBJECT_H
#define GENO_OBJECT_H

#include <GenO/global.h>
#include <GenO/objectinterface.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qmetaobject.h>

#define G_OBJECT         G_OBJECT_IMPL
#define G_RESOURCE(Name) Q_CLASSINFO("G.Resource", Name)
#define G_ID(ID)         Q_CLASSINFO("G.Id", ID)

#define G_OBJECT_IMPL \
public: \
    const QMetaObject instanceMetaObject() const override \
    { return staticMetaObject; } \
private:

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

    bool isValid() const override;
    bool isEmpty() const override;

    const QMetaObject metaObject() const override;
    virtual const QMetaObject instanceMetaObject() const;

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

    QVariant readProperty(const QString &name) const override final;
    bool writeProperty(const QString &name, const QVariant &value) override final;

    QSharedDataPointer<ObjectData> d_ptr;

    friend class ObjectData;
};

class GENO_EXPORT ObjectData : public QSharedData, public ObjectInterface
{
public:
    ObjectData() = default;
    ObjectData(const ObjectData &other) = default;

    void init(const QMetaObject &meta);

    bool hasProperty(const QString &name) const override;
    QVariant readProperty(const QString &name) const override;
    bool writeProperty(const QString &name, const QVariant &value) override;
    QStringList propertyNames() const override;

    void restoreProperties(Object *object);
    void storeProperties(const Object *object);

    bool isValid() const override { return true; }
    bool isEmpty() const override { return properties.isEmpty(); }

    inline const QMetaObject metaObject() const override
    { return m_metaObject; }

    virtual ObjectData *clone() const
    { return nullptr; }

    QMap<QString, QVariant> properties;

private:
    QMetaObject m_metaObject;
    bool m_initialized = false;

    friend class Object;
};

} // namespace GenO

Q_DECLARE_METATYPE(GenO::Object)

#endif // GENO_OBJECT_H
