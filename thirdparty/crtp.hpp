#ifndef __DUCKED_C_CRTP_HPP__
#define __DUCKED_C_CRTP_HPP__

namespace std {

	// Class providing a helper for implementing the Curiously Recursive Template Pattern (based on the one included in NamedTypes)
	template <typename T, template <typename...> class crtpType>
	struct crtp {
		constexpr T& underlying() { return static_cast<T&>(*this); }
		constexpr T const& underlying() const { return static_cast<T const&>(*this); }
	};

}

#endif // __DUCKED_C_CRTP_HPP__