#include "jsonserializationtest.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TEST_F(JsonSerializationTest, serializationTest)
{
    experiment.name = "Frankenstein Experiment";
    experiment.sample.name = "Dead Gene";

    Sample s;

    s.name = "Sample A";
    experiment.samples.append(s);

    s.name = "Sample B";
    experiment.samples.append(s);

    s.name = "Sample C";
    experiment.samples.append(s);

    QJsonObject object = experiment.toJsonObject();

    // Assert the serialized JSON structure
    ASSERT_EQ(object["name"].toString().toStdString(), "Frankenstein Experiment");

    QJsonObject sampleObject = object["sample"].toObject();
    ASSERT_EQ(sampleObject["name"].toString().toStdString(), "Dead Gene");

    QJsonArray samplesArray = object["samples"].toArray();
    ASSERT_EQ(samplesArray.size(), 3);
    ASSERT_EQ(samplesArray[0].toObject()["name"].toString().toStdString(), "Sample A");
    ASSERT_EQ(samplesArray[1].toObject()["name"].toString().toStdString(), "Sample B");
    ASSERT_EQ(samplesArray[2].toObject()["name"].toString().toStdString(), "Sample C");
}

TEST_F(JsonSerializationTest, deserializationTest)
{
    // Sample JSON data representing a complex experiment
    QByteArray jsonData = R"({
        "name": "Frankenstein Experiment",
        "sample": {
            "name": "Dead Gene"
        },
        "samples": [
            { "name": "Sample A" },
            { "name": "Sample B" },
            { "name": "Sample C" }
        ]
    })";

    QJsonObject sample = QJsonDocument::fromJson(jsonData).object();

    // Deserialize the JSON object into the ComplexExperiment object
    experiment = ComplexExperiment::fromJson(sample);

    // Assertions to validate deserialization
    ASSERT_EQ(experiment.name.toStdString(), "Frankenstein Experiment");
    ASSERT_EQ(experiment.sample.name.toStdString(), "Dead Gene");
    ASSERT_EQ(experiment.samples.size(), 3);
    ASSERT_EQ(experiment.samples.at(0).name.toStdString(), "Sample A");
    ASSERT_EQ(experiment.samples.at(1).name.toStdString(), "Sample B");
    ASSERT_EQ(experiment.samples.at(2).name.toStdString(), "Sample C");
}

