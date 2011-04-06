cmake_minimum_required(VERSION 2.6)

PROJECT(GeonectApp)

FIND_PACKAGE(Qt4 REQUIRED)

ADD_SUBDIRECTORY(nestk)
## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseNestk.cmake")

ADD_EXECUTABLE(GeonectApp GeonectApp.cpp)
TARGET_LINK_LIBRARIES(GeonectApp nestk)
