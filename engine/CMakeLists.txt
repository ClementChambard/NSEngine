cmake_minimum_required(VERSION 3.20)


# dependencies
set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


# library
file(GLOB_RECURSE SRCS    src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)

add_compile_options(-DNS_EXPORT -fPIC)

add_library(NSEngine ${SRCS} ${HEADERS})


# nsengine submodules
add_subdirectory(modules)


# link and includes
target_link_libraries(NSEngine PUBLIC
                       ${OPENGL_gl_LIBRARY}
                       ${GLEW_LIBRARIES}
                       -lSDL2
                       -lSDL2_image
                       -lSDL2_ttf
                       -pthread
                     )

target_include_directories(NSEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(NSEngine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
