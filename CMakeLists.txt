cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(FOX_ENGINE_LIB LANGUAGES C CXX)

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

add_library(FOX_ENGINE_LIB ${GAME_SOURCES})

target_include_directories(FOX_ENGINE_LIB PUBLIC "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/src")

set(DEPEND_LIBRARIES
    SDL3::SDL3
    gdi32
    opengl32
    glew_s
)

target_link_libraries(FOX_ENGINE_LIB PRIVATE ${DEPEND_LIBRARIES})

target_precompile_headers(FOX_ENGINE_LIB PRIVATE "${CMAKE_SOURCE_DIR}/pch.hpp")

install(TARGETS FOX_ENGINE_LIB
        EXPORT FOX_ENGINE_LIB_TARGET
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
