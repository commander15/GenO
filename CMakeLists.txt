cmake_minimum_required(VERSION 3.20)

project(GenO
    DESCRIPTION "The Generic Object Library"
    VERSION     1.0.0
    LANGUAGES   CXX C
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/Utils.cmake)

find_package(Qt REQUIRED COMPONENTS Core)

add_subdirectory(src)
