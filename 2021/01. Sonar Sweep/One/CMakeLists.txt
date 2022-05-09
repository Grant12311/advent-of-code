cmake_minimum_required(VERSION 3.18)

set(name SonarSweep1)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

project(${name})

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Winline -Wunreachable-code -Wshadow
        -Wconversion -Wno-switch -Wno-unused-variable -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual
        -Wsign-conversion -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference
        -Wuseless-cast -Wdouble-promotion -Wunused-variable)

option(warningsAsErrors "Treat all warnings as errors" ON)

if (warningsAsErrors)
    add_compile_options(-Werror)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(-g -pg -fsanitize=address -fsanitize=leak -fsanitize=undefined -static-libasan)
    add_link_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined -static-libasan)
else (CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-O2)
endif ()

file(GLOB files CONFIGURE_DEPENDS "include/*.h*" "src/*.c*")
add_executable(${name} ${files})

target_include_directories(${name} PRIVATE include/)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    set_property(TARGET ${name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()