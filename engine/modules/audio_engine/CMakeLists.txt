cmake_minimum_required(VERSION 3.20)

message("Using Audio engine prototype")

file(GLOB_RECURSE AUDIOSRCS src/*.h src/*.cpp include/NSAudio.h)

target_sources(NSEngine PRIVATE ${AUDIOSRCS})

target_include_directories(NSEngine PUBLIC include)

target_link_libraries(NSEngine PUBLIC -lopenal -lsndfile)
