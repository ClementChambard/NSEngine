cmake_minimum_required(VERSION 3.20)

message("Using ImGui")

# Should select the right sources depending on the backends
file(GLOB IMGUISRCS src/ImGuiContext.cpp
                    include/imgui_ns.hpp
                    src/vendor/*.h
                    src/vendor/*.cpp)

find_package(imgui REQUIRED)

target_sources(NSEngine PRIVATE ${IMGUISRCS})

target_include_directories(NSEngine PUBLIC include)

target_link_libraries(NSEngine PUBLIC imgui::imgui)

target_compile_definitions(NSEngine PRIVATE -DNSMOD_USE_IMGUI=1)
