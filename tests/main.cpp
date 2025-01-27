#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    QCoreApplication app(argc, argv);
    QLoggingCategory::setFilterRules("geno.warning=true");

    return RUN_ALL_TESTS();
}
