cmake_minimum_required(VERSION 3.0.2)
project(depth_histogram)

#OPEN CV libraries
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
set (LIBS ${LIBS} ${OpenCV_LIBS})

add_executable(DisplayHistogram main.cpp)

target_link_libraries(DisplayHistogram ${LIBS})
