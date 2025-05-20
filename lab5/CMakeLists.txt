cmake_minimum_required(VERSION 3.10)
project(lab5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/lab5.cpp
    src/graph.cpp
)

# Исполняемый файл
add_executable(lab5 ${SOURCES})

# Проверка наличия Graphviz
find_package(Graphviz)
if(Graphviz_FOUND)
    message(STATUS "Graphviz found: visualization enabled")
    target_compile_definitions(bstree PRIVATE HAS_GRAPHVIZ)
else()
    message(WARNING "Graphviz not found: visualization disabled")
endif()

# Для Windows: добавление пути к Graphviz в переменную окружения PATH
if(WIN32 AND Graphviz_FOUND)
    get_filename_component(GV_DIR "${Graphviz_DOT_EXECUTABLE}" DIRECTORY)
    set(ENV{PATH} "$ENV{PATH};${GV_DIR}")
endif()