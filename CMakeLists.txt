cmake_minimum_required(VERSION 3.10)

project(starfetch)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(CMAKE_BUILD_TYPE Debug)

add_executable(starfetch starfetch.c)

target_compile_options(starfetch PUBLIC -march=native)
target_compile_options(starfetch PUBLIC -ffast-math)

# Link the math library
target_link_libraries(starfetch m)


