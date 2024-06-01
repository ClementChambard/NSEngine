cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE SRCS    src/*.cpp src/*.c)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)

add_compile_options(-DNS_EXPORT -fPIC)

if(NS_USE_IMGUI)
  file(GLOB IMGUISRCS modules/imgui/*.cpp)
  list(APPEND SRCS ${IMGUISRCS})
  find_package(imgui REQUIRED)
  add_definitions(-DNS_USE_IMGUI=1)
  message("Using IMGUI")
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(NSEngine ${SRCS} ${HEADERS})

#set_property(TARGET NSEngine PROPERTY CXX_STANDARD 20)
#set_property(TARGET NSEngine PROPERTY CXX_STANDARD_REQUIRED True)

target_link_libraries(NSEngine PUBLIC
                       ${OPENGL_gl_LIBRARY}
                       ${GLEW_LIBRARIES}
                       -lSDL2
                       -lSDL2_image
                       -lSDL2_ttf
                       -pthread
                     )

if (NS_USE_IMGUI)
  target_link_libraries(NSEngine PUBLIC imgui::imgui)
endif()

target_include_directories(NSEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(NSEngine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

if (NS_USE_IMGUI)
  target_include_directories(NSEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/imgui")
endif()
