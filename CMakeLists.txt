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
add_subdirectory("thirdparty/doctest")
add_subdirectory("thirdparty/nanobench")
add_subdirectory("thirdparty/utfcpp")

set(CXX_STANDARD 20 REQUIRED)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-literal-suffix -std=c++20")

# add_compile_options (-flto -Wall -Wextra)

file(GLOB sources "src/*.cpp" "src/*.c" "thirdparty/DuckLibLite/src/*.cpp")
set(includes "src/" "include/" ${ext_include_dir} ${Boost_INCLUDE_DIRS} "thirdparty/" "thirdparty/doctest/doctest" "thirdparty/mdspan/include/" "thirdparty/nytl")
set(systemIncludes "include/std")
set(libraries utf8::cpp nanobench doctest ${Boost_LIBRARIES} unwind::unwind dwarf::dwarf)

add_library(DuckLib STATIC ${sources})
target_include_directories (DuckLib PUBLIC ${includes})
target_include_directories (DuckLib SYSTEM PUBLIC ${systemIncludes})
target_link_libraries(DuckLib PUBLIC ${libraries})

# if (IS_TOPLEVEL_PROJECT)
	include("thirdparty/doctest/scripts/cmake/doctest.cmake")
	enable_testing()

	add_executable(tests "tests.cpp")
	target_link_libraries(tests PUBLIC DuckLib)
	doctest_discover_tests(tests)
# endif()