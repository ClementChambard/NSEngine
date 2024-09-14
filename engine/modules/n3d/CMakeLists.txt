cmake_minimum_required(VERSION 3.20)

message("Using N3D")

file(GLOB SRCS TODO/*.cpp TODO/*.h)

target_sources(NSEngine PRIVATE ${SRCS})

# target_include_directories(NSEngine PUBLIC include)
