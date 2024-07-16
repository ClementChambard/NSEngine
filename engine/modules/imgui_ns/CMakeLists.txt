cmake_minimum_required(VERSION 3.20)

message("Using ImGui")

if(NS_PLATFORM_WND STREQUAL "SDL2")
  set(NSMOD_IMGUI_WND_VENDORSRC src/vendor/imgui_impl_sdl.h src/vendor/imgui_impl_sdl.cpp)
elseif(NS_PLATFORM_WND STREQUAL "X11")
  set(NSMOD_IMGUI_WND_VENDORSRC src/imgui_impl_x11.h src/imgui_impl_x11.cpp)
endif()

# TODO: Depends on rendering backend
set(NSMOD_IMGUI_RENDERER_VENDORSRC src/vendor/imgui_impl_opengl3.h src/vendor/imgui_impl_opengl3.cpp src/vendor/imgui_impl_opengl3_loader.h)

# Should select the right sources depending on the backends
file(GLOB IMGUISRCS src/ImGuiContext.cpp
                    include/imgui_ns.hpp
                    ${NSMOD_IMGUI_RENDERER_VENDORSRC}
                    ${NSMOD_IMGUI_WND_VENDORSRC})

find_package(imgui REQUIRED)

target_sources(NSEngine PRIVATE ${IMGUISRCS})

target_include_directories(NSEngine PUBLIC include)

target_link_libraries(NSEngine PUBLIC imgui::imgui)
