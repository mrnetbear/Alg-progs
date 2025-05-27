cmake_minimum_required(VERSION 3.14)
project(labmsu
    VERSION 1.0.0
    DESCRIPTION "Lab about connected sets"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/labmsu.cpp
    src/connectedSets.cpp
)

set(TESTS
    src/tests.cpp
    src/connectedSets.cpp
)

option(LABMSU_BUILD_TESTS "Build labmsu tests" OFF)

# Исполняемый файл
if (LABMSU_BUILD_TESTS)
    add_executable(labmsu_tests ${TESTS})
else()
    add_executable(labmsu ${SOURCES})
endif()
