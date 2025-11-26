cmake_minimum_required(VERSION 3.28)
project(mastermind LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

add_executable(mastermind src/main.cpp)
target_include_directories(mastermind PUBLIC "lib")
add_library(lib lib/mastermind.cpp)
target_link_libraries(mastermind PUBLIC lib)
target_link_libraries(mastermind PRIVATE sfml-graphics) 
target_compile_features(mastermind PRIVATE cxx_std_17)
