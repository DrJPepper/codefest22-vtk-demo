cmake_minimum_required(VERSION 3.16)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS "-Wall -O2 -fcompare-debug-second")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

# Include Qt basic functions
include(QtCommon)

# Basic information about project

project(joelsvtkdemo VERSION 1.0)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

# Set additional project information
set(COMPANY "JCP353")
set(COPYRIGHT "Copyright (c) 2022 Joel Pepper")
set(IDENTIFIER "com.pepper.Vtkdemo")

set(SOURCE_FILES
    src/main.cpp
    src/mainwindow.cpp
)

add_project_meta(META_FILES_TO_INCLUDE)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(VTK REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${VTK_INCLUDE_DIRS} ${JSON_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${OS_BUNDLE} ${SOURCE_FILES} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_precompile_headers(${PROJECT_NAME} INTERFACE QtWidgets.h)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${VTK_LIBRARIES})
