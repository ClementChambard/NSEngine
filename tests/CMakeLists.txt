cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
file(GLOB_RECURSE INCLUDES src/*.hpp src/*.h src/*.inl)

add_compile_options(-DNS_IMPORT)
 
add_executable(tests ${SRCS} ${INCLUDES})

target_link_libraries(tests PRIVATE NSEngine)
