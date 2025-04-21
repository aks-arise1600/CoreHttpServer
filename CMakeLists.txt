cmake_minimum_required(VERSION 3.10)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(CoreHttpServer
  VERSION
    1.0
  DESCRIPTION
    "Qt/C++ based HTTP Server"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Network)

if(Qt5_FOUND)
    message(STATUS "${PROJECT_NAME} Found Qt5 version: ${Qt5_VERSION}")
else()
    message(FATAL_ERROR "Qt5 not Found !!")
endif()


set(SRC_FILES
                main.cpp
                HTTP_Handler.cpp
                )

set(HEADER_FILES
                httplib.h
                HTTP_Handler.h
                )


add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
add_definitions(-DQT_NO_VERSION_TAGGING)

target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Core)
