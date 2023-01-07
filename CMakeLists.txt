cmake_minimum_required(VERSION 3.8)
project(DuckLib LANGUAGES CXX)

#Force colors in Ninja
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
	 if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		 add_compile_options (-fdiagnostics-color=always)
	 elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		 add_compile_options (-fcolor-diagnostics)
	 endif ()
endif ()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(IS_TOPLEVEL_PROJECT TRUE)
else()
	set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(DUCKLIB_BUILD_TESTING "Build DuckLib tests " ${IS_TOPLEVEL_PROJECT})

if(NOT DEFINED DUCKLIB_CORE_FOUND OR NOT ${DUCKLIB_CORE_FOUND})
	add_subdirectory("thirdparty/core")
endif()

set(CXX_STANDARD 23 REQUIRED)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -std=c++23")

# add_compile_options (-flto -Wall -Wextra)

file(GLOB sources "src/*.cpp" "src/*.c")
set(includes "include/" "thirdparty")
set(libraries dl::dlcore)

add_library(DuckLib_Stream STATIC "src/stream.cpp")
target_include_directories (DuckLib_Stream PUBLIC ${includes})
target_link_libraries(DuckLib_Stream PUBLIC ${libraries})

add_library(dl::stream ALIAS DuckLib_Stream)

if (DUCKLIB_BUILD_TESTING)
	if(NOT DEFINED DUCKLIB_CORE_FOUND OR NOT ${DUCKLIB_CORE_FOUND})
		include("thirdparty/core/cmake/testing.cmake") # This needs to become shorter!
	endif()

	add_executable(tests-stream "tests.cpp")
	target_link_libraries(tests-stream PUBLIC dl::stream)
	doctest_discover_tests(tests-stream)
endif()