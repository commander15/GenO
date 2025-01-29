#ifndef AGENCY_H
#define AGENCY_H

#include <GenO/object.h>

typedef GenO::Object Object;

class DataObject : public GenO::Object
{
    Q_GADGET
    Q_PROPERTY(QString id MEMBER id)

public:
    QString id;
};

class Coordinate : public Object
{
    G_OBJECT(Coordinate)

    Q_GADGET
    Q_PROPERTY(double longitude MEMBER longitude)
    Q_PROPERTY(double latitude MEMBER latitude)

public:
    double longitude = 0.0;
    double latitude = 0.0;
};

G_REGISTER(Coordinate)

class Contact : public Object
{
    G_OBJECT(Contact)

    Q_GADGET
    Q_PROPERTY(QString email MEMBER email)
    Q_PROPERTY(QString phone READ phone WRITE setPhone)
    Q_PROPERTY(QStringList phones MEMBER phones)
    Q_PROPERTY(int poBox MEMBER poBox)

public:
    QString phone() const
    { return !phones.isEmpty() ? phones.first() : QString(); }
    void setPhone(const QString &phone)
    { phones.clear(); if (!phone.isEmpty()) phones.append(phone); }

    QString email;
    QStringList phones;
    int poBox = 0;
};

G_REGISTER(Contact)

class Location : public DataObject
{
    G_OBJECT(Location)

    Q_GADGET
    Q_PROPERTY(QString city MEMBER city)
    Q_PROPERTY(QString country MEMBER country)
    Q_PROPERTY(Coordinate coordinates MEMBER coordinates)

public:
    QString city;
    QString country;
    Coordinate coordinates;
};

G_REGISTER(Location)

class Role : public DataObject
{
    G_OBJECT(Role)

    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)

public:
    QString name;
};

G_REGISTER(Role)

class Skill : public DataObject
{
    G_OBJECT(Skill)

    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)

public:
    QString name;
};

G_REGISTER(Skill)

class Employee : public DataObject
{
    G_OBJECT(Employee)

    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString firstName MEMBER firstName)
    Q_PROPERTY(QString gender MEMBER gender)
    Q_PROPERTY(Contact contact MEMBER contact)
    Q_PROPERTY(Role role MEMBER role)
    Q_PROPERTY(QList<Skill> skills MEMBER skills)

public:
    QString name;
    QString firstName;
    QString gender;
    Contact contact;
    Role role;
    QList<Skill> skills;
};

G_REGISTER(Employee)

class Agency : public DataObject
{
    G_OBJECT(Agency)

    Q_GADGET
    Q_PROPERTY(Location location MEMBER location)
    Q_PROPERTY(Contact contact MEMBER contact)
    Q_PROPERTY(QList<Employee> employees MEMBER employees)

public:
    Location location;
    Contact contact;
    QList<Employee> employees;
};

G_REGISTER(Agency)

#endif // AGENCY_H
