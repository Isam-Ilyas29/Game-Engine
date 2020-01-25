cmake_minimum_required(VERSION 3.13)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
project(GladApp CXX)

add_subdirectory(Libraries/glfw)
add_subdirectory(Libraries/glad)
add_subdirectory(Libraries/stb_image)
add_subdirectory(Libraries/glm)

#Source files
set(SOURCES 
    Source/main.cpp
    Source/Rendering/shader.h
    Source/Rendering/shader.cpp
    Source/Rendering/graphic.h
    Source/Rendering/graphic.cpp
    Source/Rendering/texture.h
    Source/Rendering/texture.cpp
    Source/Environment/environment.cpp
    Source/Environment/environment.h
    Source/Input/keyboard.h
    Source/Input/keyboard.cpp
    Source/Input/mouse.h
    Source/Input/mouse.cpp
    Source/Camera/camera.h
    Source/Camera/camera.cpp
    Source/Camera/camera_property.h
    Source/Camera/camera_property.cpp
    Source/Input/input_respondant.h
    Source/Input/input_respondant.cpp
    Source/Input/input.h
    Source/Input/input.cpp
    Source/Context/context.h
    Source/Context/context.cpp
    Source/Game/gameloop.h
    Source/Game/gameloop.cpp
)
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

set(EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/ExeDirectory")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_PATH}")

#Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<BOOL:$<CXX_COMPILER_ID:MSVC>>>:-Wall -Wextra>
)

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${EXECUTABLE_PATH}")
endforeach()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

#Libraries
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

#Windows + Linux Support
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
     set(PLATFORM_LIBS OpenGL32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_LIBS
    GL
    X11
    Xrandr
    Xi
    m
    stdc++fs
)
endif()

target_link_libraries (${PROJECT_NAME} ${PLATFORM_LIBS})