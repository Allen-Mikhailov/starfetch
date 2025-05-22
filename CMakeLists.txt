cmake_minimum_required(VERSION 3.10)

project(starfetch)

add_executable(starfetch starfetch.c)

# Link the math library
target_link_libraries(starfetch m)


