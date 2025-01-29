#include "objecttest.h"

#include <QMetaObject>

TEST(ObjectTest, metaObjectsRetrieve)
{
    Experiment experiment;
    const char *className = Experiment::staticMetaObject.className();

    const QMetaObject *meta = experiment.metaObject();
    ASSERT_EQ(meta->className(), className);

    const QMetaObject *iMeta = experiment.instanceMetaObject();
    ASSERT_EQ(iMeta->className(), className);
}

TEST(ObjectTest, simplePropertyRetrieve)
{
    Experiment experiment;
    const char *name = "DNA Hack";
    experiment.name = name;

    ASSERT_EQ(experiment.property("name").toString().toStdString(), name);
}

TEST(ObjectTest, advancedPropertyRetrieve)
{
    Experiment experiment;
    const char *sample = "GEN-X";
    experiment.sample.name = sample;
    ASSERT_EQ(experiment.property("sample.name").toString().toStdString(), sample);

    sample = "GEN-Y";
    experiment.setProperty("sample.name", QString::fromLatin1(sample));
    ASSERT_EQ(experiment.sample.name.toStdString(), sample);
}

TEST(ObjectTest, propertiesTransferTest)
{
    Experiment experiment;
    experiment.name = "X";

    Sample sample;
    GenO::Object::transferProperties(experiment, sample);

    ASSERT_EQ(experiment.name, sample.name);
}

TEST(ObjectTest, mutationTest)
{
    Experiment experiment;
    experiment.name = "X";

    Sample sample = experiment.to<Sample>();

    ASSERT_EQ(experiment.name, sample.name);
}
