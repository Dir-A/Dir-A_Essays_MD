cmake_minimum_required(VERSION 3.20)

project(MyWindow)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
add_executable("${PROJECT_NAME}" main.cpp mywindow.cpp mywindow.ui mywindow.qrc)
target_link_libraries("${PROJECT_NAME}" Qt6::Widgets)