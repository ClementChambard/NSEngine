cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")
set(PROJECT_URL "https://github.com/ClementChambard/NSEngine")

add_compile_options(-Wall -Wextra -Werror)

if(CMAKE_BUILD_TYPE EQUAL "Debug")
  add_compile_options(-D_DEBUG)
else()
  add_compile_options(-O4)
endif()

project(NSEngine VERSION 1.1)

add_subdirectory(engine)
add_subdirectory(testbed)
add_subdirectory(tests)
