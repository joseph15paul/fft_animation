cmake_minimum_required(VERSION 3.31)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(animatedFFT)
include_directories("./include/" "./libs/")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
				"src/*.cpp"
				"src/*.cc"
				"src/*.cxx"
				"src/*.c"
)
add_executable(animatedFFT ${SOURCES})

find_package(OpenGL)
target_link_libraries(animatedFFT PRIVATE GL)

find_package(glfw3 REQUIRED)
target_link_libraries(animatedFFT PRIVATE glfw)

include(FetchContent)

FetchContent_Declare(
		cmrc
		GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
		GIT_TAG master
)
FetchContent_MakeAvailable(cmrc)
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
				"src/shaders/*.vert"
				"src/shaders/*.frag"
				"src/shaders/*.geom"
				"src/shaders/*.comp"
				"src/shaders/*.glsl"
)
cmrc_add_resource_library(
				shaders_rc
				NAMESPACE shaders
				${SHADER_FILES}
)

target_link_libraries(animatedFFT PRIVATE shaders_rc)

