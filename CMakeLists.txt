cmake_minimum_required(VERSION 3.0)

project(MyCProject C)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enforce C90 (equivalent to C89)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Gather all .c and .h files recursively
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/*.c"
    "${CMAKE_SOURCE_DIR}/*.h"
)

# Create an executable
add_executable(my_program ${SRC_FILES})

# Optionally enforce strict C89/C90 mode with compiler-specific flags
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(my_program PRIVATE -std=c90 -pedantic -Wall -Wextra)
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(my_program PRIVATE /std:c89)
endif()
