cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(Game LANGUAGES C CXX)

set (BUILD_SHARED_LIBS OFF)
set (BUILD_ONLY_DEBUG_SFML OFF)
set (SFML_BUILD_AUDIO ON)
set (SFML_BUILD_GRAPHICS ON)
set (SFML_BUILD_WINDOW ON)
set (SFML_BUILD_NETWORK OFF)
if(WIN32)
    set (SFML_STATIC_LIBRARIES ON)
    set (SFML_USE_STATIC_STD_LIBS OFF)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sdl EXCLUDE_FROM_ALL)
add_definitions(-DSFML_STATIC)
add_subdirectory(${CMAKE_SOURCE_DIR}/SFML)

add_definitions(-DGLEW_STATIC)
add_subdirectory("${CMAKE_SOURCE_DIR}/GLEW/build/cmake" "${CMAKE_CURRENT_BINARY_DIR}/GLEW")
add_compile_definitions(
    GLM_FORCE_EXPLICIT_CTOR
    GLM_FORCE_XYZW_ONLY
    GLM_FORCE_SIZE_T_LENGTH
)
add_subdirectory("${CMAKE_SOURCE_DIR}/glm")

file(GLOB_RECURSE GAME_SOURCE src/*.cpp) 
file(GLOB_RECURSE GAME_HEADERS src/*.hpp) 

set(GAME_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    ${GAME_SOURCE}
    ${GAME_HEADERS}
)

add_executable(Game "${GAME_SOURCES}")

target_include_directories(Game PUBLIC "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/src")

set(DEPEND_LIBRARIES
    SDL3::SDL3
	sfml-system
	sfml-graphics
	sfml-window
	sfml-audio
    gdi32
    opengl32
    glew_s
)

target_link_libraries(Game PRIVATE ${DEPEND_LIBRARIES})

target_precompile_headers(Game PRIVATE "${CMAKE_SOURCE_DIR}/pch.hpp")
