cmake_minimum_required(VERSION 3.23)
project(raygame)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")

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

set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow -pedantic)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    raylib 
    raylib_cpp 
    fmt
    Argumentum::headers
)

