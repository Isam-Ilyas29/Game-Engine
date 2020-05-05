cmake_minimum_required(VERSION 3.13)
project(imgui CXX)

# Unzip imgui
unzip(imgui.zip "${CMAKE_CURRENT_SOURCE_DIR}")

# Set imgui target
add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
