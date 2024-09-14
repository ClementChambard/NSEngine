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

set(NS_PLATFORM_WND "X11")

# MODULES TESTS
set(NS_USE_AUDIO_PROTO ON CACHE BOOL "enable audio engine prototype" FORCE)

# nsengine submodules
add_subdirectory(modules)

if(NS_PLATFORM_WND STREQUAL "SDL2")
  message("Using SDL2 as windowing backend")
  set(NS_PLATFORM_WND_LIBS "-lSDL2")
  target_compile_definitions(NSEngine PRIVATE -DNS_WND_BACKEND_SDL2)
elseif(NS_PLATFORM_WND STREQUAL "X11")
  message("Using X11 as windowing backend")
  set(NS_PLATFORM_WND_LIBS "-lX11")
  target_compile_definitions(NSEngine PRIVATE -DNS_WND_BACKEND_X11)
else()
  message("Unknown windowing backend")
endif()

# link and includes
target_link_libraries(NSEngine PUBLIC
                       ${OPENGL_gl_LIBRARY}
                       ${GLEW_LIBRARIES}
                       ${NS_PLATFORM_WND_LIBS}
                       -pthread
                     )

target_include_directories(NSEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(NSEngine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
