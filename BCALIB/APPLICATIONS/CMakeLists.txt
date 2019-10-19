cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

set(APPNAME NeutronW_Simulation)

include_directories(${PROJECT_SOURCE_DIR}/../src/include/Common)
include_directories(${PROJECT_SOURCE_DIR}/../src/include/APPLICATIONS)

foreach(_conftype ${CMAKE_CONFIGURATION_TYPES})
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_BINDIR}")
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}")
endforeach()

add_executable(${APPNAME} ${PROJECT_SOURCE_DIR}/../src/source/APPLICATIONS/${APPNAME}.cpp)
target_link_libraries(${APPNAME} Common ${Geant4_LIBRARIES})

install(TARGETS ${APPNAME} RUNTIME DESTINATION bin)