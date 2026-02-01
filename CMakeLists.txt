cmake_minimum_required(VERSION 3.16)
project(EyescireEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(EyescireEditor
    main.cpp
)

target_link_libraries(EyescireEditor
    Qt6::Widgets
)