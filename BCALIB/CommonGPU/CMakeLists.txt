cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

set(LIBNAME CommonGPU)

find_package(CUDA REQUIRED)

#set(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_CXX_FLAGS "-O2 -Wall")
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_75,code=sm_75;-std=c++11;-x=cu)
set(CUDA_64_BIT_DEVICE_CODE ON)
#set(GENCODE ${GENCODE} -gencode arch=compute_70,code=sm_70)
#set(GENCODE ${GENCODE} -gencode arch=compute_50,code=sm_50)
#set(GENCODE ${GENCODE} -gencode arch=compute_35,code=sm_35)
#set(GENCODE ${GENCODE} -gencode arch=compute_30,code=sm_30)
#set(GENCODE ${GENCODE} -gencode arch=compute_20,code=sm_20)

include_directories(${PROJECT_SOURCE_DIR}/../src/include/${LIBNAME})
include_directories(${PROJECT_SOURCE_DIR}/../src/include/Common)

file(GLOB sources_CommonGPU ${PROJECT_SOURCE_DIR}/../src/source/${LIBNAME}/*.cu)
file(GLOB headers_CommonGPU ${PROJECT_SOURCE_DIR}/../src/include/${LIBNAME}/*.cuh)

cuda_add_library(${LIBNAME} STATIC ${sources_CommonGPU} ${headers_CommonGPU})
target_link_libraries(${LIBNAME} ${UTILLIBS} ${Geant4_LIBRARIES} Common ${CUDA_cudadevrt_LIBRARY})

install(TARGETS ${LIBNAME} DESTINATION lib)
