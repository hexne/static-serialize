cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30.0 AND CMAKE_VERSION VERSION_LESS_EQUAL 3.31.7)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
elseif(CMAKE_VERSION EQUAL 3.31.8)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0.0 AND CMAKE_VERSION VERSION_LESS_EQUAL 4.0.2)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0.3) # AND CMAKE_VERSION VERSION_LESS_EQUAL 4.1.0)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
endif()
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_MODULE_STD 1)


project(untitled)

add_compile_options(
    -fparameter-reflection
    -freflection-latest
)

include_directories("~/clang-latest/include/c++/v1/")

add_library(Module)
target_sources(Module
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        serialize.cppm
)

target_compile_options(Module PRIVATE
    -fparameter-reflection
    -freflection-latest
    -stdlib=libc++
    -nostdinc++
    -resource-dir "${CUSTOM_CLANG_PATH}/lib/clang/21"
    -isystem "${CUSTOM_CLANG_PATH}/include/c++/v1"
    -isystem "${CUSTOM_CLANG_PATH}/lib/clang/21/include"
)

add_executable(untitled main.cpp)
target_link_libraries(untitled
    Module
)
