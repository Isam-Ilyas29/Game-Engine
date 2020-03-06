cmake_minimum_required(VERSION 3.13)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
project(GladApp CXX)

add_subdirectory(Libraries/glfw)
add_subdirectory(Libraries/glad)
add_subdirectory(Libraries/stb_image)
add_subdirectory(Libraries/glm)
add_subdirectory(Libraries/imgui)

# Disable GLFW compiler warnings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.*pp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

# ifdefs for DEBUG and RELEASE
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:DEBUG_MODE>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:RELEASE_MODE>")

# ExeDirectory
set(EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/ExeDirectory")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_PATH}")

# Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<BOOL:$<CXX_COMPILER_ID:MSVC>>>:-Wall -Wextra>
)
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
        $<$<NOT:$<BOOL:$<CXX_COMPILER_ID:MSVC>>>:-Werror>
	)
endif()

# Disable GLM compiler warnings
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS GLM_FORCE_RADIANS)

target_include_directories(${PROJECT_NAME} INTERFACE SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glm/include")

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${EXECUTABLE_PATH}")
endforeach()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Libraries
target_link_libraries(${PROJECT_NAME} 
    glfw
    glad
    stb_image
    glm
    imgui
)

# Windows + Linux Support
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