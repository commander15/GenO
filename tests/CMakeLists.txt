enable_testing()

find_package(GTest REQUIRED)

add_executable(GenOTest
    main.cpp
    agency.h agency.cpp
    objecttest.cpp objecttest.h
    jsonserializationtest.cpp jsonserializationtest.h
    jsonstresstest.cpp jsonstresstest.h
)

target_compile_definitions(GenOTest PRIVATE
    ROOT_DIR="${PROJECT_SOURCE_DIR}"
    TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/testdata"
)

target_link_libraries(GenOTest PRIVATE GTest::gtest Qt::Test GenO)
