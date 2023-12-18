cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(FnF LANGUAGES C CXX)

set (BUILD_SHARED_LIBS OFF)

add_definitions(-DGLEW_STATIC)
add_compile_definitions(
    GLM_FORCE_EXPLICIT_CTOR
    GLM_FORCE_XYZW_ONLY
    GLM_FORCE_SIZE_T_LENGTH
)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sdl EXCLUDE_FROM_ALL)
add_subdirectory("${CMAKE_SOURCE_DIR}/extern/GLEW/build/cmake" "${CMAKE_CURRENT_BINARY_DIR}/GLEW")

add_subdirectory("${CMAKE_SOURCE_DIR}/extern/glm")

file(GLOB_RECURSE GAME_SOURCE src/*.cpp) 
file(GLOB_RECURSE GAME_HEADERS src/*.hpp) 

set(GAME_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    ${GAME_SOURCE}
    ${GAME_HEADERS}
)

add_executable(FnF "${GAME_SOURCES}")

target_include_directories(FnF PUBLIC "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/src")

set(DEPEND_LIBRARIES
    SDL3::SDL3
    gdi32
    opengl32
    glew_s
)

target_link_libraries(FnF PRIVATE ${DEPEND_LIBRARIES})

target_precompile_headers(FnF PRIVATE "${CMAKE_SOURCE_DIR}/pch.hpp")
