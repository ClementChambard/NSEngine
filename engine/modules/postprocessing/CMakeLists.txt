cmake_minimum_required(VERSION 3.20)

message("Using postprocessing prototype")

file(GLOB SRCS TODO/*.cpp TODO/*.h)

file(GLOB_RECURSE SAMPLES TODO/samples/*.cpp TODO/samples/*.h)

target_sources(NSEngine PRIVATE ${SRCS} ${SAMPLES})

# target_include_directories(NSEngine PUBLIC include)
