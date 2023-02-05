cmake_minimum_required(VERSION 2.6)

project(yolo)

#change to your own path
##################################################
set(OpenCV_DIR "E:\\opencv\\opencv\\build")  
set(TRT_DIR "E:\\TensorRT-8.5.1.7")  
set(Dirent_INCLUDE_DIRS "H:\\1\\cf\\include")
##################################################

add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

# setup CUDA
set(CUDA_DIR "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v11.6")
find_package(CUDA REQUIRED)
message(STATUS "    libraries: ${CUDA_LIBRARIES}")
message(STATUS "    include path: ${CUDA_INCLUDE_DIRS}")
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${Dirent_INCLUDE_DIRS}) 

#change to your GPU own compute_XX
###########################################################################################
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-std=c++11;-g;-G;-gencode;arch=compute_86;code=sm_86)
###########################################################################################

####
enable_language(CUDA)  # add this line, then no need to setup cuda path in vs
####
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${TRT_DIR}\\include)

# -D_MWAITXINTRIN_H_INCLUDED for solving error: identifier "__builtin_ia32_mwaitx" is undefined
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -D_MWAITXINTRIN_H_INCLUDED")

# setup opencv
find_package(OpenCV QUIET
    NO_MODULE
    NO_DEFAULT_PATH
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${TRT_DIR}\\lib)

add_executable(yolo ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/dxgi-cap.cpp ${PROJECT_SOURCE_DIR}/pid.cpp ${PROJECT_SOURCE_DIR}/yolo-trt.cpp ${PROJECT_SOURCE_DIR}/include/dxgi-cap.h ${PROJECT_SOURCE_DIR}/include/pid.h ${PROJECT_SOURCE_DIR}/include/yolo-trt.h) 

target_link_libraries(yolo "nvinfer" "nvinfer_plugin")  
target_link_libraries(yolo ${OpenCV_LIBS})     
target_link_libraries(yolo ${CUDA_LIBRARIES})  
target_link_libraries(yolo Threads::Threads)     
