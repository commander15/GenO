enable_testing()

add_executable(GenOTest
    main.cpp
    objecttest.h objecttest.cpp
    jsonserializationtest.h jsonserializationtest.cpp
    jsonstresstest.h jsonstresstest.cpp
    agency.h agency.cpp
)

target_compile_definitions(GenOTest PRIVATE ROOT_DIR="${PROJECT_SOURCE_DIR}")

target_link_libraries(GenOTest PRIVATE GTest::gtest Qt::Test GenO)
