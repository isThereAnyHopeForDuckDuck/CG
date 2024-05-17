cmake_minimum_required(VERSION 3.0)

project(gms101hw)

find_package(OpenCV REQUIRED)
#OpenCV_LIBS OpenCV_INCLUDE_DIRS

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

set(EXE_NAME "app")
#set(SRCDIR_HW "./hw/Homework1/Assignment1")
set(SRCDIR_HW "./hw/Homework2")

add_subdirectory(${SRCDIR_HW})

