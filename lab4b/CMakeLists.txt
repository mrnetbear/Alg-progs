cmake_minimum_required(VERSION 3.10)
project(lab4b)

# Устанавливаем стандарт C++ (C++11 или новее)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для включения/выключения поддержки Graphviz
option(WITH_GRAPHVIZ "Enable Graphviz visualization support" ON)

# Основной исполняемый файл
add_executable(lab4b
    src/lab4b.cpp
    src/llrb.cpp
)

# Проверяем наличие Graphviz, если опция включена
if(WITH_GRAPHVIZ)
    find_package(Graphviz)
    if(Graphviz_FOUND)
        message(STATUS "Graphviz found - visualization support enabled")
        target_compile_definitions(llrb_tree PRIVATE HAS_GRAPHVIZ)
    else()
        message(WARNING "Graphviz not found - visualization support disabled")
    endif()
endif()

# Устанавливаем выходной каталог для исполняемых файлов
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Настройки для более строгих предупреждений компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lab4b PRIVATE -Wall -Wextra -pedantic)
endif()