#include "objecttest.h"

TEST_F(ObjectTest, metaObjectsRetrieve)
{
    const char *className = Sample::staticMetaObject.className();

    const QMetaObject meta = sample.metaObject();
    ASSERT_EQ(meta.className(), className);

    const QMetaObject iMeta = sample.instanceMetaObject();
    ASSERT_EQ(iMeta.className(), className);
}

TEST_F(ObjectTest, propertyRetrieve)
{
    const char *name = "GEN-X";
    sample.name = name;

    ASSERT_EQ(sample.property("name").toString().toStdString(), name);
}
