cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BCAPKASpec)

option(WITH_GEANT4_UIVIS "Build with geant4 UI and vis driver" ON)

if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

message("The geant4 use file is: " ${Geant4_USE_FILE})
include(${Geant4_USE_FILE})

message("The current project source direction is: " ${PROJECT_SOURCE_DIR})
message("The project binary direction is: " ${PROJECT_BINARY_DIR})

if(NOT DEFINED CMAKE_INSTALL_BINDIR)
	set(CMAKE_INSTALL_BINDIR "bin")
endif()

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR "lib")
endif()

#Set the output path
set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

foreach(_conftype ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${_conftype} _conftype_uppercase)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_conftype_uppercase}
    "${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_BINDIR}"
    )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_conftype_uppercase}
    "${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}"
    )
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_conftype_uppercase}
    "${BASE_OUTPUT_DIRECTORY}/${_conftype}/${CMAKE_INSTALL_LIBDIR}"
    )
endforeach()



set(UTILLIBS MiniUtilities)
set(BCALIBS Common)
set(BCABINS APPLICATIONS ANALYSIS)

set(APPS NeutronW_Simulation)
set(ANALYS NeutronW_Analysis New_NeutronW_Analysis)

foreach(UTILLIBNAME ${UTILLIBS})
  add_subdirectory(${PROJECT_SOURCE_DIR}/LIB/${UTILLIBNAME})
endforeach()

foreach(LIBNAME ${BCALIBS})
  add_subdirectory(${PROJECT_SOURCE_DIR}/BCALIB/${LIBNAME})
endforeach()

foreach(BINNAME ${BCABINS})
  add_subdirectory(${PROJECT_SOURCE_DIR}/BCALIB/${BINNAME})
endforeach()

message("CMAKE_CURRENT_LIST_FILE is : " ${CMAKE_CURRENT_LIST_FILE})

message("_geant4_thisdir : " ${_geant4_thisdir})
message("Geant4_INCLUDE_DIR: " ${Geant4_INCLUDE_DIR})

add_custom_target(BCAPKASpec DEPENDS ${UTILLIBS} ${BCALIBS} ${APPS} ${ANALYS})