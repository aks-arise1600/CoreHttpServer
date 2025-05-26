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
#find_package(PkgConfig REQUIRED)

if(Qt5_FOUND)
    message(STATUS "${PROJECT_NAME} Found Qt5 version: ${Qt5_VERSION}")
else()
    message(FATAL_ERROR "Qt5 not Found !!")
endif()

find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}")
else()
    message(FATAL_ERROR "OpenSSL not found!")
endif()

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)


set(SRC_FILES
                main.cpp
                HTTP_Handler.cpp
                )

set(HEADER_FILES
                httplib.h
                HTTP_Handler.h
                htmls.h
                )

set(QRC_FILES
                resources.qrc
                )


add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES} ${QRC_FILES})
add_definitions(-DQT_NO_VERSION_TAGGING)

target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Core)
target_link_libraries(${PROJECT_NAME} ssl crypto)

