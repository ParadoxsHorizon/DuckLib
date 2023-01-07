#define TESTING_DEFINE_MAIN // generate a main function to run the tests
#include <ducklib>
#include <testing>
#include <stream>
#include <stacktrace>

using namespace dl::builtin;
using namespace dl::literals;

// Captures the console output (contains an open block)
#define CAPTURE_CONSOLE std::stringstream capture; \
/* capture(std::cout) */{\
	std::streambuf* std__cout_buf = std::cdbg.rdbuf();\
	std::cdbg.rdbuf(capture.rdbuf());\
	defer { std::cdbg.rdbuf(std__cout_buf); };\

TEST_CASE("Terminal colors") {
	std::cout << std::term::info{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::warning{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::error{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::fatal{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::debug{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::fg::red << "Hello World!" << std::term::style::reset << std::endl;
}

TEST_CASE("std::cdbg") {
	variant<int, string, char> v = "5";

	{CAPTURE_CONSOLE
		std::cdbg << 5 << std::endl;
		}
		CHECK(capture.str() == "(int) 5\n");
	}
	
	{CAPTURE_CONSOLE
		std::cdbg << v << std::endl;
		}
		CHECK(capture.str().starts_with("(dl::variant<int, dl::basic_string<char, std::char_traits<char>, std::allocator<char> >, char>) at 0x") == true);
	}
}