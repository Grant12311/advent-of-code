cmake_minimum_required(VERSION 3.18)

set(name Hydrothermal_Venture_Part_Two)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_C_FLAGS_DEBUG "-g -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")

set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

project(${name})

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wcast-align -Wconversion -Wdouble-promotion -Wduplicated-cond -Wextra -Wformat-signedness -Wlogical-op -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wshadow -Wsign-conversion -Wunreachable-code -Wuseless-cast)

option(warningsAsErrors "Treat all warnings as errors" OFF)

if (warningsAsErrors)
    add_compile_options(-Werror)
endif()

option(enableAddressSanitizer "Enable address sanitizer" ON)
option(enableLeakSanitizer "Enable leak sanitizer" ON)
option(enableUBSanitizer "Enable UB sanitizer" ON)

if (enableAddressSanitizer)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

if (enableLeakSanitizer)
    add_compile_options(-fsanitize=leak)
    add_link_options(-fsanitize=leak)
endif ()

if (enableUBSanitizer)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif ()

file(GLOB files CONFIGURE_DEPENDS "include/*.h*" "src/*.c*")
add_executable(${name} ${files})

target_include_directories(${name} PRIVATE include/)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    set_property(TARGET ${name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()