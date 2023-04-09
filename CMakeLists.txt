cmake_minimum_required(VERSION 2.6)

project(yolo)

add_definitions(-std=c++11)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
# add_definitions("-Wall -g")
find_package(CUDA REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
# include and link dirs of cuda and tensorrt, you need adapt them if yours are different
# cuda
include_directories(C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/include)
link_directories(C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64)
# tensorrt
include_directories(E:/TensorRT-8.5.1.7/include/)
link_directories(E:/TensorRT-8.5.1.7/lib/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(yolo ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(yolo nvinfer)
target_link_libraries(yolo nvinfer_plugin)
target_link_libraries(yolo cudart)

target_link_libraries(yolo ${OpenCV_LIBS})

add_definitions(-O2 -pthread)