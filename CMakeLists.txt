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
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(NeutronTungsten NeutronW_Main.cpp ${sources} ${headers})
target_link_libraries(NeutronTungsten ${Geant4_LIBRARIES})


message(${Geant4_LIBRARIES})


message("CMAKE_CURRENT_LIST_FILE is : " ${CMAKE_CURRENT_LIST_FILE})

message("_geant4_thisdir : " ${_geant4_thisdir})
message("Geant4_INCLUDE_DIR: " ${Geant4_INCLUDE_DIR})


add_custom_target(BCAPKASpec DEPENDS NeutronTungsten)


install(TARGETS NeutronTungsten DESTINATION bin)