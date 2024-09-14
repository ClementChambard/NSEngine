cmake_minimum_required(VERSION 3.20)


# Applications should set the variables NS_USE_[...] to activate a submodule
# This file will add subdirectories corresponding to each activated submodules
# NS_USE_ALL_SUBMODULES will set all variables (used for debugging the library itself)

if(NS_USE_IMGUI OR NS_USE_ALL_SUBMODULES)
  add_subdirectory(imgui_ns)
  target_compile_definitions(NSEngine PRIVATE -DNSMOD_USE_IMGUI=1)
endif()

if (NS_USE_AUDIO_PROTO OR NS_USE_ALL_SUBMODULES)
  add_subdirectory(audio_engine)
  target_compile_definitions(NSEngine PRIVATE -DNSMOD_USE_AUDIO_PROTO=1)
endif()

if (NS_USE_N3D OR NS_USE_ALL_SUBMODULES)
  add_subdirectory(n3d)
  target_compile_definitions(NSEngine PRIVATE -DNSMOD_USE_N3D=1)
endif()

if (NS_USE_POSTPROC_PROTO OR NS_USE_ALL_SUBMODULES)
  add_subdirectory(postprocessing)
  target_compile_definitions(NSEngine PRIVATE -DNSMOD_USE_POSTPROC_PROTO=1)
endif()

# ...


# Add the source files specific to submodule management

target_sources(NSEngine PRIVATE shared_src/modules_init.cpp shared_src/modules_run.cpp)
