cmake_minimum_required(VERSION 2.6)

PROJECT(GeonectApp)

SET(EXECUTABLE_OUTPUT_PATH ${GeonectApp_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${GeonectApp_BINARY_DIR}/lib)
OPTION(NESTK_USE_OPENNI "Use OpenNI drivers" 1)

ADD_SUBDIRECTORY(nestk)
## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseNestk.cmake")

# FIXME: on Mac, the dylib rpath points to current directory.
if(APPLE)
add_custom_command(TARGET GeonectApp POST_BUILD
COMMAND cp ${LIBRARY_OUTPUT_PATH}/*.dylib ${EXECUTABLE_OUTPUT_PATH}/
)
endif()
