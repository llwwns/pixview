cmake_minimum_required(VERSION 3.15)
project(pixview)

IF (WIN32)
ELSE()
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ENDIF()
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories("third-party/toml11")

add_executable(pixview WIN32
        main.cpp
        mainwindow.h
        mainwindow.cpp
        library.h
        library.cpp
        imageworker.h
        imageworker.cpp
        setting.h
        setting.cpp
        imageview.h
        imageview.cpp
        viewstate.h
        )

target_link_libraries(pixview
        Qt5::Widgets)
