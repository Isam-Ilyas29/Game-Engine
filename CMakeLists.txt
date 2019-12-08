cmake_minimum_required(VERSION 3.13)
project(GladApp CXX)

add_subdirectory(Libraries/glfw)

set(SOURCES Source/main.cpp)
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)
add_dependencies(${PROJECT_NAME} glfw)
