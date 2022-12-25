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

find_package(Boost)
include("cmake/FindUnwind.cmake")
include("cmake/FindDWARF.cmake")
set(FELSPAR_CHECK_TESTS OFF CACHE STRING "Turn off to stop tests from being built and run")
add_subdirectory("thirdparty/doctest")
add_subdirectory("thirdparty/nanobench")
add_subdirectory("thirdparty/utfcpp")

set(CXX_STANDARD 23 REQUIRED)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -std=c++23")

# add_compile_options (-flto -Wall -Wextra)

file(GLOB sources "src/*.cpp" "src/*.c" "thirdparty/DuckLibLite/src/*.cpp" "thirdparty/strnatcmp.cpp")
set(includes "src/" "include/" ${ext_include_dir} ${Boost_INCLUDE_DIRS} "thirdparty/" "thirdparty/doctest/doctest" "thirdparty/mdspan/include/" "thirdparty/nytl")
set(systemIncludes "include/std")
set(libraries utf8::cpp nanobench doctest ${Boost_LIBRARIES} unwind::unwind dwarf::dwarf)

add_library(DuckLib_core STATIC ${sources})
target_include_directories (DuckLib_core PUBLIC ${includes})
target_include_directories (DuckLib_core SYSTEM PUBLIC ${systemIncludes})
target_link_libraries(DuckLib_core PUBLIC ${libraries})

add_library(dl::dlcore ALIAS DuckLib_core)

if (DUCKLIB_BUILD_TESTING)
	include("thirdparty/doctest/scripts/cmake/doctest.cmake")
	enable_testing()

	add_executable(tests-core "tests.cpp")
	target_link_libraries(tests-core PUBLIC DuckLib_core)
	doctest_discover_tests(tests-core)
endif()