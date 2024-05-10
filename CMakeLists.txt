# tell CMake what version of CMake we require
# CMake builds makefiles and msbuilds
# ninja does better at determining parallelism
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# declare project
project(LoxInterpreter)

# Optionally specify languages in use
# Default mode is C and C++ are both enabled
# We can be more strict
enable_language(CXX)

# Use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# I want to write compliant C++
# I don't want to opt into vendor extensions
set(CMAKE_CXX_EXTENSIONS Off)

# There are extensions for your IDE that give you autocomplete
# It tells your computer information on the build, so extensions
# can give better autocomplete support
# We want CMake to generate a compilation database for us (enables better autocomplete)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)
# export CMAKE_EXPORT_COMPILE_COMMANDS=1
# it'll always then do this for you

# Build type in-between debug + release
# does not support assert-style of checking pre-conditions
#set(CMAKE_BUILD_TYPE RelWithDebInfo)

# we'll specify a target name, what we want to invoke
# and then all the source files associated with it
add_executable(lox)
target_sources(lox PRIVATE src/driver.cpp)

# building our library in isolation
add_library(mmapfile)
target_sources(mmapfile PRIVATE src/mmapfile.cpp)

# tell lox that we depend on this library
target_link_libraries(lox PRIVATE mmapfile)
