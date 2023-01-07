#ifndef __DUCKED_C_NYTL__
#define __DUCKED_C_NYTL__

#include "defer.hpp"

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