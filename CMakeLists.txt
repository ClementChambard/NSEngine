cmake_minimum_required(VERSION 3.10)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_URL "https://github.com/ClementChambard/NSEngine")

project(NSEngine VERSION 1.0)

file(GLOB_RECURSE SRCS    src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library("${PROJECT_NAME}" ${SRCS} ${HEADERS})

target_link_libraries(NSEngine PUBLIC
                       ${OPENGL_gl_LIBRARY}
                       ${GLEW_LIBRARIES}
                       -lSDL2
                       -lSDL2_ttf
                       -lopenal
                       -lsndfile
                     )

target_include_directories(NSEngine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
