cmake_minimum_required(VERSION 3.10)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(NSEngine VERSION 1.0)
set(PROJECT_URL "https://github.com/ClementChambard/NSEngine")
add_executable("${PROJECT_NAME}" test.cpp)

target_include_directories("${PROJECT_NAME}" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
