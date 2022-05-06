#include <stacktrace>
#include <stream>

// Function which generates a string containing a stack trace on demand
std::string std::stacktrace(backward::Printer p, backward::StackTrace st) {
	st.load_here();
	st.load_from(st[1].addr); // Don't include this function in stack traces
	// Print the stack trace to a string
	std::ostringstream out;
#if (!defined(WIN32) && !defined(_WIN32) && !defined(__WIN32)) || defined(__CYGWIN__) // Not windows
	auto modeCache = std::term::get_control_mode();
	std::term::set_control_mode(std::term::control::Force);
#endif
	p.print(st, out);
#if (!defined(WIN32) && !defined(_WIN32) && !defined(__WIN32)) || defined(__CYGWIN__) // Not windows
	std::term::set_control_mode(modeCache);
#endif
	return out.str();
}
