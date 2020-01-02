cmake_minimum_required(VERSION 3.13)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
project(GladApp CXX)

add_subdirectory(Libraries/glfw)
add_subdirectory(Libraries/glad)
add_subdirectory(Libraries/stb_image)
add_subdirectory(Libraries/glm)

set(SOURCES 
    Source/main.cpp
    Source/rendering/shader.h
    Source/rendering/shader.cpp
    Source/rendering/graphics.h
    Source/rendering/graphics.cpp
    Source/rendering/texture.h
    Source/rendering/texture.cpp
    Source/environment/environment.cpp
    Source/environment/environment.h
    Source/window.h
    Source/window.cpp
    Source/input/keyboard.h
    Source/input/keyboard.cpp
    Source/input/mouse.h
    Source/input/mouse.cpp
)
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

set(EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/ExeDirectory")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_PATH}")
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${EXECUTABLE_PATH}")
endforeach()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} 
    glfw
    glad
    stb_image
    glm
)
add_dependencies(${PROJECT_NAME} 
    glfw
    glad
    stb_image
    glm
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
     set(PLATFORM_LIBS OpenGL32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_LIBS
    GL
    X11
    Xrandr
    Xi
    m
)
endif()

target_link_libraries (${PROJECT_NAME} ${PLATFORM_LIBS})