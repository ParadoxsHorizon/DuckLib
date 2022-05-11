#ifndef __DUCKED_C_NYTL__
#define __DUCKED_C_NYTL__

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



#include <nytl/flags.hpp>
namespace std {
	template<typename T>
	class flags : public nytl::Flags<T> {
		using Base = nytl::Flags<T>;
	public:
		using Base::Base;
		constexpr flags(const flags&) = default;
		constexpr flags(flags&&) = default;
		constexpr flags& operator=(const flags&) = default;
		constexpr flags& operator=(flags&&) = default;
	};

	constexpr auto invert_flags = nytl::invertFlags;
}

#define FLAG_OPS(T) NYTL_FLAG_OPS(T)



#include <nytl/clone.hpp>
#include <nytl/nonCopyable.hpp>
namespace std {
	using nytl::clone;

	template<typename T> inline auto clone_move(T& obj) { return nytl::cloneMove(obj); }
	
	template<typename Derived, typename... Bases> using clone_movable = nytl::DeriveCloneMovable<Derived, Bases...>; 
	template<typename Derived, typename... Bases> using cloneable = nytl::DeriveCloneable<Derived, Bases...>; 

	using non_copyable = nytl::NonCopyable;
	using non_movable = nytl::NonMovable;

	struct non_copy_moveable: public non_copyable, public non_movable {
		using non_copyable::non_copyable;
		using non_movable::non_movable;
	};
}

#endif // __DUCKED_C_NYTL__