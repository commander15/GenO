#ifndef OBJECTTEST_H
#define OBJECTTEST_H

#include <gtest/gtest.h>

#include <GenO/object.h>

class Sample : public GenO::Object
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)

    G_OBJECT(Sample)
    G_RESOURCE("/samples")

public:
    QString name;
};

G_REGISTER(Sample)

class Experiment : public GenO::Object
{
    Q_GADGET
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(Sample sample MEMBER sample)

    G_OBJECT(Experiment)
    G_RESOURCE("/experiments")

public:
    QString name;
    Sample sample;
};

G_REGISTER(Experiment)

class ComplexExperiment;
class ComplexExperiment : public Experiment
{
    Q_GADGET
    Q_PROPERTY(QList<Sample> samples MEMBER samples)
    Q_PROPERTY(QList<ComplexExperiment> subExperiments MEMBER subExperiments)

    G_OBJECT(ComplexExperiment)
    G_RESOURCE("/complex/experiments")

public:
    QList<Sample> samples;
    QList<ComplexExperiment> subExperiments;
};

G_REGISTER(ComplexExperiment)

#endif // OBJECTTEST_H
