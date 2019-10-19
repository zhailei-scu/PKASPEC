cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

foreach(_conftype ${CMAKE_CONFIGURATION_TYPES})
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_BINDIR}")
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}")
endforeach()

foreach(LIBNAME ${BCALIBS})
	include_directories(${PROJECT_SOURCE_DIR}/../src/include/${LIBNAME})
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/../src/include/ANALYSIS)

set(ANALYS NeutronW_Analysis)

foreach(ANALYSNAME ${ANALYS})
	add_executable(${ANALYSNAME} ${PROJECT_SOURCE_DIR}/../src/source/ANALYSIS/${ANALYSNAME}.cpp)
	target_link_libraries(${ANALYSNAME} ${BCALIBS} ${Geant4_LIBRARIES})

	install(TARGETS ${ANALYSNAME} RUNTIME DESTINATION bin)
endforeach()