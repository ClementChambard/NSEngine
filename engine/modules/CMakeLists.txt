cmake_minimum_required(VERSION 3.20)


# Applications should set the variables NS_USE_[...] to activate a submodule
# This file will add subdirectories corresponding to each activated submodules

if(NS_USE_IMGUI)
  add_subdirectory(imgui_ns)
endif()

# ...


# Add the source files specific to submodule management

target_sources(NSEngine PRIVATE shared_src/modules_init.cpp)
