enable_testing()

add_executable(GenOTest
    main.cpp
    objecttest.h objecttest.cpp
)

target_link_libraries(GenOTest PRIVATE GTest::gtest Qt::Test GenO)
