cmake_minimum_required(VERSION 2.6)

PROJECT(GeonectApp)

SET(EXECUTABLE_OUTPUT_PATH ${GeonectApp_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${GeonectApp_BINARY_DIR}/lib)

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)

SET(GeonectApp_SOURCES
	GeonectApp.cpp
	RawImagesWindow.cpp
	GuiController.cpp
	GLWidget.cpp)

SET(GeonectApp_MOC_HEADERS
	GuiController.h
  	RawImagesWindow.h
	GLWidget.h)

INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

INCLUDE(${QT_USE_FILE})

ADD_SUBDIRECTORY(nestk)
## nestk/UseNestk.cmake.in defines variable to use nestk.
## It itself includes nestk/deps/cmake/UseEmbeddedNestkDeps.cmake that
## contains most of the cmake find packages commands.
INCLUDE("${nestk_BINARY_DIR}/UseNestk.cmake")

QT4_WRAP_CPP(GeonectApp_SOURCES ${GeonectApp_MOC_HEADERS})

QT4_WRAP_UI(uic_sources
  RawImagesWindow.ui
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(GeonectApp ${GeonectApp_SOURCES} ${uic_sources})

TARGET_LINK_LIBRARIES(GeonectApp nestk ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

# FIXME: on Mac, the dylib rpath points to current directory.
if(APPLE)
    add_custom_command(TARGET GeonectApp POST_BUILD
        COMMAND cp ${LIBRARY_OUTPUT_PATH}/*.dylib ${EXECUTABLE_OUTPUT_PATH}/
    )
endif()
