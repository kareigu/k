cmake_minimum_required(VERSION 3.23)
project(raygame)

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(GRAPHICS GRAPHICS_API_OPENGL_ES2)

if(WIN32)
    set(CC clang)
    set(CXX clang++)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-diagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

if(WIN32)
    add_compile_options(-fansi-escape-codes)
endif()

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG v4.5.0
)
FetchContent_MakeAvailable(raylib_cpp)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.0.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    Argumentum
    GIT_REPOSITORY https://github.com/mmahnic/argumentum.git
    GIT_TAG v0.3.2
)
FetchContent_MakeAvailable(Argumentum)

FetchContent_Declare(
    Result
    GIT_REPOSITORY https://github.com/bitwizeshift/result.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(Result)

FetchContent_Declare(
    FastNoise2
    GIT_REPOSITORY https://github.com/Auburn/FastNoise2.git
    GIT_TAG v0.10.0-alpha
)
FetchContent_MakeAvailable(FastNoise2)

include_directories(src/)
set(SOURCES 
    src/main.cpp
    src/core/config.cpp
    src/core/input.cpp
    src/core/assert.cpp
    src/core/game_state.cpp
    src/core/game_loop.cpp
    src/gfx/renderer.cpp
    src/gfx/tiles.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow -pedantic)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    raylib 
    raylib_cpp 
    fmt
    Argumentum::headers
    Result
    FastNoise2
)

