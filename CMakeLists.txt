cmake_minimum_required(VERSION 3.31)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(animatedFFT)
include_directories("./Include/" "./Libs/")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
				"Src/*.cpp"
				"Src/*.cc"
				"Src/*.cxx"
				"Src/*.c"
)
add_executable(animatedFFT ${SOURCES})

