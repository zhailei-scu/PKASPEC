cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

foreach(_conftype ${CMAKE_CONFIGURATION_TYPES})
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_BINDIR}")
	link_directories("${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}")
endforeach()

foreach(LIBNAME ${BCALIBS})
	include_directories(${PROJECT_SOURCE_DIR}/../src/include/${LIBNAME})
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/../src/include/APPLICATIONS)

foreach(APPNAME ${APPS})
	add_executable(${APPNAME} ${PROJECT_SOURCE_DIR}/../src/source/APPLICATIONS/${APPNAME}.cpp)
	target_link_libraries(${APPNAME} ${UTILLIBS} ${BCALIBS} ${Geant4_LIBRARIES})

	install(TARGETS ${APPNAME} RUNTIME DESTINATION bin)
endforeach()