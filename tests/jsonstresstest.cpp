#include "jsonstresstest.h"

#include "agency.h"

TEST_F(JsonStressTest, LargeDatasetSerialization)
{
}

TEST_F(JsonStressTest, LargeDatasetDeserialization)
{
    QJsonArray array = loadJsonFile("testdata/agency_data.json").toArray();
    QJsonObject jsonObject = array.at(1).toObject(); // We just process part of data

    Agency agency = Agency::fromJson(jsonObject);

    ASSERT_EQ(agency.id.toStdString(), "6672825c-652a-4075-b0ad-854397433aec");

    Location location = agency.location;
    ASSERT_EQ(location.id.toStdString(), "92fb4a30-dcba-494b-8702-c996a17103ed");
    ASSERT_EQ(location.city.toStdString(), "Неготино");
    ASSERT_EQ(location.country.toStdString(), "Macedonia");

    Coordinate coords = location.coordinates;
    ASSERT_EQ(coords.longitude, 21.3300873);
    ASSERT_EQ(coords.latitude, 41.0377245);

    Contact contact = agency.contact;
    ASSERT_EQ(contact.email.toStdString(), "rtattershaw1@free.fr");
    ASSERT_EQ(contact.phones.size(), 2);
    ASSERT_EQ(contact.phones[0].toStdString(), "+55 177 829 6817");
    ASSERT_EQ(contact.phones[1].toStdString(), "+1 624 766 4511");
    ASSERT_EQ(contact.poBox, 1236);

    ASSERT_EQ(agency.employees.size(), 2);
    Employee employee;

    employee = agency.employees[0];
    ASSERT_EQ(employee.id.toStdString(), "1894811e-6974-41b1-b195-db8b9dbbb2bf");
    ASSERT_EQ(employee.name.toStdString(), "Betancourt");
    ASSERT_EQ(employee.gender.toStdString(), "Female");
    ASSERT_EQ(employee.firstName.toStdString(), "Pansy");
    ASSERT_EQ(employee.role.id.toStdString(), "bd4eae8f-1f09-451e-aa01-ffbe7829b192");
    ASSERT_EQ(employee.role.name.toStdString(), "Estimator");
    ASSERT_EQ(employee.contact.email.toStdString(), "pbetancourt0@multiply.com");
    ASSERT_EQ(employee.contact.phone().toStdString(), "+48 959 668 9544");
    ASSERT_EQ(employee.skills.size(), 2);
    ASSERT_EQ(employee.skills[0].name.toStdString(), "Blogger");
    ASSERT_EQ(employee.skills[1].name.toStdString(), "Workplace Safety");

    employee = agency.employees[1];
    ASSERT_EQ(employee.id.toStdString(), "756b64d2-db0e-4774-88f8-814939fd16dd");
    ASSERT_EQ(employee.name.toStdString(), "Whacket");
    ASSERT_EQ(employee.gender.toStdString(), "Male");
    ASSERT_EQ(employee.firstName.toStdString(), "Haywood");
    ASSERT_EQ(employee.role.id.toStdString(), "56834548-8d04-408a-9d8a-02a976ee1e02");
    ASSERT_EQ(employee.role.name.toStdString(), "Supervisor");
    ASSERT_EQ(employee.contact.email.toStdString(), "hwhacket1@accuweather.com");
    ASSERT_EQ(employee.contact.phone().toStdString(), "+55 858 102 4672");
    ASSERT_EQ(employee.skills.size(), 4);
    ASSERT_EQ(employee.skills[0].name.toStdString(), "VGA");
    ASSERT_EQ(employee.skills[1].name.toStdString(), "Pymol");
    ASSERT_EQ(employee.skills[2].name.toStdString(), "Life Cycle Assessment");
    ASSERT_EQ(employee.skills[3].name.toStdString(), "Commercial Property Owners");
}

