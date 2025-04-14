cmake_minimum_required(VERSION 3.28)
project(mastermind LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(mastermind src/main.cpp)
target_include_directories(mastermind PUBLIC "lib")
add_library(lib lib/mastermind.cpp)
target_link_libraries(mastermind PUBLIC lib)
target_compile_features(mastermind PRIVATE cxx_std_17)
target_link_libraries(mastermind PRIVATE SFML::Graphics)
