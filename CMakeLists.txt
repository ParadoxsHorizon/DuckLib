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
add_subdirectory(thirdparty/subprocess)

set(CXX_STANDARD 23 REQUIRED)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -std=c++23")

# add_compile_options (-flto -Wall -Wextra)

file(GLOB sources "src/*.cpp" "src/*.c")
set(includes "include/" "thirdparty")
set(libraries dl::dlcore subprocess)

add_library(DuckLib_Desktop_Core STATIC "src/stream.cpp")
target_include_directories (DuckLib_Desktop_Core PUBLIC ${includes})
target_link_libraries(DuckLib_Desktop_Core PUBLIC ${libraries})

add_library(dl::desktop::core ALIAS DuckLib_Desktop_Core)

if (DUCKLIB_BUILD_TESTING)
	if(NOT DEFINED DUCKLIB_CORE_FOUND OR NOT ${DUCKLIB_CORE_FOUND})
		include("thirdparty/core/cmake/testing.cmake") # This needs to become shorter!
	endif()

	add_executable(tests-desktop "tests.cpp")
	target_link_libraries(tests-desktop PUBLIC dl::desktop::core)
	doctest_discover_tests(tests-desktop)
endif()