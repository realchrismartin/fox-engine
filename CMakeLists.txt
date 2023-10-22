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

add_definitions(-DSFML_STATIC)
add_subdirectory(${CMAKE_SOURCE_DIR}/SFML)


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
	sfml-system
	sfml-graphics
	sfml-window
	sfml-audio
)

target_link_libraries(Game PRIVATE ${DEPEND_LIBRARIES})
