cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

find_package(CUDA REQUIRED)

#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CUDA_NVCC_FLAGS -gencode arch=compute_35,code=sm_35)
#set(GENCODE ${GENCODE} -gencode arch=compute_70,code=sm_70)
#set(GENCODE ${GENCODE} -gencode arch=compute_50,code=sm_50)
#set(GENCODE ${GENCODE} -gencode arch=compute_35,code=sm_35)
#set(GENCODE ${GENCODE} -gencode arch=compute_30,code=sm_30)
#set(GENCODE ${GENCODE} -gencode arch=compute_20,code=sm_20)

foreach(_conftype ${CMAKE_CONFIGURATION_TYPES})
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_BINDIR}")
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}")
endforeach()

foreach(LIBNAME ${BCALIBS})
	include_directories(${PROJECT_SOURCE_DIR}/../src/include/${LIBNAME})
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/../src/include/ANALYSIS_GPU)

foreach(ANALYSNAME ${ANALYSIS_GPU})
	cuda_add_executable(${ANALYSNAME} ${PROJECT_SOURCE_DIR}/../src/source/ANALYSIS_GPU/${ANALYSNAME}.cu)
	target_link_libraries(${ANALYSNAME} ${UTILLIBS} ${BCALIBS} ${Geant4_LIBRARIES} ${CUDA_cudadevrt_LIBRARY})

	install(TARGETS ${ANALYSNAME} RUNTIME DESTINATION bin)
endforeach()