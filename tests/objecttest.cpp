#include "objecttest.h"

#include <QMetaObject>

TEST_F(ObjectTest, metaObjectsRetrieve)
{
    const char *className = Experiment::staticMetaObject.className();

    const QMetaObject *meta = experiment.metaObject();
    ASSERT_EQ(meta->className(), className);

    const QMetaObject *iMeta = experiment.instanceMetaObject();
    ASSERT_EQ(iMeta->className(), className);
}

TEST_F(ObjectTest, simplePropertyRetrieve)
{
    const char *name = "DNA Hack";
    experiment.name = name;

    ASSERT_EQ(experiment.property("name").toString().toStdString(), name);
}

TEST_F(ObjectTest, advancedPropertyRetrieve)
{
    const char *sample = "GEN-X";
    experiment.sample.name = sample;
    ASSERT_EQ(experiment.property("sample.name").toString().toStdString(), sample);

    sample = "GEN-Y";
    experiment.setProperty("sample.name", QString::fromLatin1(sample));
    ASSERT_EQ(experiment.sample.name.toStdString(), sample);
}
