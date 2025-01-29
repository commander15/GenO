#ifndef JSONSTRESSTEST_H
#define JSONSTRESSTEST_H

#include <gtest/gtest.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonStressTest : public ::testing::Test
{
protected:
    QJsonValue loadJsonFile(const QString &filePath)
    {
        QFile file(QStringLiteral(ROOT_DIR) + '/' + filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::runtime_error(("Failed to open file: " + filePath.toStdString()).c_str());
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isObject())
            return doc.object();
        else if (doc.isArray())
            return doc.array();
        else
            return QJsonValue();

        return doc.object();
    }
};

#endif // JSONSTRESSTEST_H
