cmake_minimum_required(VERSION 3.13)
project(glad C)

set(SOURCES
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)

list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")