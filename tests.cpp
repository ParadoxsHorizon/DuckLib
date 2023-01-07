#define TESTING_DEFINE_MAIN // generate a main function to run the tests
#include <ducklib>
#include <testing>
#include <stream>
#include <subprocess>

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

TEST_CASE( "Subprocess" ) {
	// capture output. You can do this syntax if you have C++20
	auto process = std::run({"echo", "hello", "world"}, {
		// make true to throw exception
		.check = false,
		.cout = std::pipe_option::pipe,
	});

	CHECK(process.cout == "hello world\n");

	// quick echo it, doesn't capture
	std::run({"echo", "hello", "world"});

	// simplest capture output.
	process = std::run({"echo", "hello", "world"},
		std::run_builder().cout(std::pipe_option::pipe));
	CHECK(process.cout == "hello world\n");

	// simplest sending data example
	process = std::run({"cat"},
		std::run_builder().cin("hello world\n"));
	CHECK(process.cout.empty() == true);

	// simplest send & capture
	process = std::run({"cat"},
		std::run_builder().cin("hello world").cout(std::pipe_option::pipe));
	CHECK(process.cout == "hello world");

	// capture stderr too.
	process = std::run({"echo", "hello", "world"},
		std::run_builder().cerr(std::pipe_option::pipe)
		.cout(std::pipe_option::pipe)
		.check(true) // will throw called_process_error if returncode != 0.
	);
	CHECK(process.cout == "hello world\n");
	CHECK(process.cerr.empty() == true);
}


#include <thread>
TEST_CASE( "Popen" ) {
	// simplest example
    // capture is enabled by default
    std::popen popen = std::run_builder({"echo", "hello", "world"})
        .cout(std::pipe_option::pipe).popen();
    array<byte, 1024> buf = (byte)0; // initializes everything to 0
    std::pipe_read(popen.cout, buf);
	CHECK(string::view((const char*)buf.data()) == "hello world\n"sv);
    // std::cout << (char*)buf.data();
    // the destructor will call wait on your behalf.
    popen.close();


    // communicate with data
    popen = std::run_builder({"cat"}).cin(std::pipe_option::pipe)
        .cout(std::pipe_option::pipe).popen();
    /*  if we write more data than the buffer, we would dead lock if the subprocess
        is deadlocked trying to write. So we spin a new thread for writing. When
        you provide buffers for cin, internally the library spins it's own thread.
    */
    std::thread write_thread([&]() {
        std::pipe_write(popen.cin, "hello world\n", std::strlen("hello world\n"));
        // no more data to send. If we don't close we may run into a deadlock as
        // we are looking to read for more.
        popen.close_cin();
    });

    for (auto& c : buf)
        c = (byte)0;

    std::pipe_read(popen.cout, buf);
	CHECK(string::view((const char*)buf.data()) == "hello world\n"sv);
    popen.close();
    if (write_thread.joinable())
        write_thread.join();
}