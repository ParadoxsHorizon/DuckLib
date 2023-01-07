#ifndef ___DUCKED_C_DEFER_HPP___
#define ___DUCKED_C_DEFER_HPP___

// Defer (https://stackoverflow.com/questions/32432450/what-is-standard-defer-finalizer-implementation-in-c)
#include <nytl/scope.hpp>
namespace std {
	template<invocable F, bool OnSuccess = true, bool OnException = true> using scope_guard = nytl::ScopeGuard<F, OnSuccess, OnException>;
	template<invocable F> using success_guard = nytl::SuccessGuard<F>;
	template<invocable F> using except_guard = nytl::ExceptionGuard<F>;
} // Drag nytl::scope_guard into the std namespace
#ifndef defer
namespace dl {
	struct ___ducked_c_defer_dummy___ {};
	template <std::invocable F> std::scope_guard<F> operator<<(___ducked_c_defer_dummy___, F f) { return {std::move(f)}; }
}
#define DEFER_(LINE) zz_defer##LINE
#define DEFER(LINE) DEFER_(LINE)
#define defer auto DEFER(__LINE__) = ::dl::___ducked_c_defer_dummy___{} << [&]()
#endif // defer

#endif // ___DUCKED_C_DEFER_HPP___