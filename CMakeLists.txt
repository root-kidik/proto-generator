cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Config)

project(proto_generator DESCRIPTION ${PROJECT_DESCRIPTION})

find_package(Clang)
list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR} ${CLANG_CMAKE_DIR})
include(AddLLVM)
include(AddClang)
set(LLVM_LINK_COMPONENTS Support)

add_subdirectory(Jinja2Cpp)

add_subdirectory(src/${PROJECT_NAME})
