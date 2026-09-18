cmake_minimum_required(VERSION 3.20)

project(QtLibTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Network Sql Test Widgets)

include(CTest)

add_executable(TestHumanFormats
    TestHumanFormats.cpp
    ../Utils.cpp
)

target_include_directories(TestHumanFormats PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(TestHumanFormats PRIVATE
    Qt6::Network
    Qt6::Sql
    Qt6::Test
    Qt6::Widgets
)

if(MSVC)
    target_compile_options(TestHumanFormats PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(TestHumanFormats PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wold-style-cast
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Werror
    )
endif()

add_test(NAME TestHumanFormats COMMAND TestHumanFormats)
