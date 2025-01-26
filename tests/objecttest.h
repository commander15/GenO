#ifndef OBJECTTEST_H
#define OBJECTTEST_H

#include <gtest/gtest.h>

#include <GenO/object.h>

class Sample : public GenO::Object
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)

    G_OBJECT
    G_RESOURCE("Sample")

public:
    QString name;
};

class ObjectTest : public testing::Test
{
public:
    Sample sample;
};

#endif // OBJECTTEST_H
