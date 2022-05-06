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
	std::streambuf* std__cout_buf = std::cout.rdbuf();\
	std::cout.rdbuf(capture.rdbuf());\
	defer { std::cout.rdbuf(std__cout_buf); };\


TEST_CASE("Type Demangling") {
	CHECK( std::demangle(typeid(int)) == "int");
	CHECK( std::demangle(typeid(std::stacktrace)) == "std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > (backward::Printer, backward::StackTrace)");
	CHECK( std::demangle(typeid(std::cout)) == "std::__1::basic_ostream<char, std::__1::char_traits<char> >");
}

TEST_CASE("Terminal colors") {
	std::cout << std::term::info{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::warning{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::error{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::fatal{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::debug{} << "Hello World!" << std::term::style::reset << std::endl;
	std::cout << std::term::fg::red << "Hello World!" << std::term::style::reset << std::endl;
}

TEST_CASE("Extended size types") {
	CHECK( sizeof(rational) == 64 );
	CHECK( sizeof(i8)   == 8 / 8 );
	CHECK( sizeof(i16)  == 16 / 8 );
	CHECK( sizeof(i32)  == 32 / 8 );
	CHECK( sizeof(i64)  == 64 / 8 );
	CHECK( sizeof(i128) == 128 / 8 + 16 );   // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(i256) == 256 / 8 + 16 );   // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(i512) == 512 / 8 + 16 );   // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(i1024) == 1024 / 8 + 16 ); // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(u8)   == 8 / 8 );
	CHECK( sizeof(u16)  == 16 / 8 );
	CHECK( sizeof(u32)  == 32 / 8 );
	CHECK( sizeof(u64)  == 64 / 8 );
	CHECK( sizeof(u128) == 128 / 8 );        // Extended types are documented to be a word larger than they should be // TODO: why is this not a word longer?
	CHECK( sizeof(u256) == 256 / 8 + 16 );   // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(u512) == 512 / 8 + 16 );   // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(u1024) == 1024 / 8 + 16 ); // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(f32)  == 32 / 8 );
	CHECK( sizeof(f64)  == 64 / 8 );
	CHECK( sizeof(f128) == 128 / 8 + 16 ); // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(f256) == 256 / 8 + 32 ); // Extended types are documented to be a word larger than they should be // TODO: Why is this two words longer?

	CHECK( i8w::min == -128 );
	CHECK( i8w::max == 127 );
	CHECK( u8w::min == 0 );
	CHECK( u8w::max == 255 );
	CHECK( i1024w::min == -0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_i1024 );
	CHECK( i1024w::max == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_i1024 );
	CHECK( u1024w::min == 0 );
	CHECK( u1024w::max == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF_u1024 );
	CHECK( std::isnan(f32w::nan) == true );

	CHECK( std::numeric_limits<i8w>::is_specialized == true );
	CHECK( std::numeric_limits<i8w>::min() == -128 );

	rational rat = 7;
	rat /= 3;
	rat *= 2;
	CHECK( double(rat) == 14.0 / 3 );

	iarb inf = 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	CHECK( inf == 0x400000000000000000000000000000000000000000000000000000000000000_i1024 );

	// std::cout << inf << std::endl;
	// std::cout << 0xC05748BBFB5ACD7E5A77DC03D9EC7D8BB957C1B95D9B206090D83FD1B67433CE83EAD7376CCFD612C72901F4CE0A2E07E322D438EA4F34647555D62D04140E1084E999BB4CD5F947A76674009E2318549FD102C5F7596EDC332A0DDEE3A355186B9A046F0F96A279C1448A9151549DC663DA8A6E89CF8F511BAED6450DA2C1CB_i1024 << std::endl;
	// std::cout << sizeof(iarb) << std::endl;
}

TEST_CASE("StackTrace") {
	CHECK( std::stacktrace().empty() == false );
}

TEST_CASE("String Properties") {
	// Fails due to minor inprecision
	// string n = "127.8889";
	// CAPTURE_CONSOLE
	// 	std::cout << float(n);
	// }
	// CHECK( capture.str() == n );

	string s = "bおb";
	CHECK( s.size() == 5 );
	CHECK( s.capacity() >= 5 );
	CHECK( s.data() != nullptr );
	CHECK( *s.data() == 'b' );

	CHECK( s.front() == 'b' );
	s.front() = 'B';
	CHECK( s.front() == 'B' );

	CHECK( s.back() == 'b' );
	s.back() = 'B';
	CHECK( s.back() == 'B' );

	CHECK( s.contains("B") == true );
	CHECK( s.contains("お") == true );
	CHECK( s.starts_with("B") == true );
	CHECK( s.ends_with("B") == true );

	CHECK( s.valid(std::utf8) == true );
	CHECK( s.size(std::utf8) == 3 );
	CHECK( s.front(std::utf8) == "B" );
	CHECK( s.back(std::utf8) == "B" );

	s = "Hello World";
	CHECK( s.caseless_equal("hello world") == true );
	CHECK( s.toupper(std::utf8) == "HELLO WORLD" );

	s = "apple";
	CHECK( s.caseless_less("banana") == true );
	CHECK( s.caseless_less("BANANA") == true );

	s = "こんにちは世界";
	CHECK( s == "こんにちは世界" );
	CHECK( s.tolower(std::utf8) == "こんにちは世界" );
	CHECK( s.caseless_equal(std::utf8, "こんにちは世界") == true);

	s = "óó";
	CHECK( s.toupper(std::utf8) == "ÓÓ" );

	s = "    		";
	CHECK( s.trim().empty() == true );

	string::view sv = "こんにちは世界"sv;
	auto u8iter = sv.begin(std::utf8);
	CHECK( string::view(*(++u8iter)) == "ん"sv );
}

TEST_CASE( "Endians" ) {
	// 8bit
	CHECK( std::swap_endian((u8) 0xff) == 0xff);
	CHECK( std::swap_endian((u8) 0x35) == 0x35);
	CHECK( std::swap_endian((u8) 0x43) == 0x43);
	CHECK( std::swap_endian((u8) 0x48) == 0x48);
	CHECK( std::swap_endian((u8) 0x8c) == 0x8c);
	CHECK( std::swap_endian((u8) 0x65) == 0x65);

	// 16bit
	CHECK( std::swap_endian((u16) 0xff00) == 0x00ff);
	CHECK( std::swap_endian((u16) 0x3543) == 0x4335);
	CHECK( std::swap_endian((u16) 0x488c) == 0x8c48);
	CHECK( std::swap_endian((u16) 0x65c9) == 0xc965);
	CHECK( std::swap_endian((u16) 0x2370) == 0x7023);
	CHECK( std::swap_endian((u16) 0xc929) == 0x29c9);

	// 32bit
	CHECK( std::swap_endian((u32) 0xffff0000) == 0x0000ffff);
	CHECK( std::swap_endian((u32) 0x3543488c) == 0x8C484335);
	CHECK( std::swap_endian((u32) 0x65c92370) == 0x7023C965);
	CHECK( std::swap_endian((u32) 0xc9292370) == 0x702329C9);
	CHECK( std::swap_endian((u32) 0x8b945300) == 0x0053948B);
	CHECK( std::swap_endian((u32) 0xdd4c4216) == 0x16424CDD);

	// 64bit
	CHECK( std::swap_endian((u64) 0xffffffff00000000) == 0x00000000ffffffff);
	CHECK( std::swap_endian((u64) 0x3543488c08C4843b) == 0x3b84c4088c484335);
	CHECK( std::swap_endian((u64) 0x65) 				== 0x6500000000000000);

	// benchmarks
	std::benchmark_header("Endians");
	BENCHMARK("8bit") {
		std::do_not_optimize(
			std::swap_endian((u8) 0xff)
		);
	};
	BENCHMARK("16bit") {
		std::do_not_optimize(
			std::swap_endian((u16) 0xff00)
		);
	};
	BENCHMARK("32bit") {
		std::do_not_optimize(
			std::swap_endian((u32) 0xffff0000)
		);
	};
	BENCHMARK("64bit") {
		std::do_not_optimize(
			std::swap_endian((u64) 0xffffffff00000000)
		);
	};
}


// -- Enum Wrapper Class --


namespace std {
	struct enum_conversion_error : public std::runtime_error { using std::runtime_error::runtime_error; };

	template<typename T>
	struct is_enum_class : std::false_type {};

	template<typename T, int64_t index>
	struct enum_name { constexpr static const char* value = "INVALID"; };

	template<typename T, int64_t index>
	struct enum_name_caseless { constexpr static const char* value = "INVALID"; };

	template<typename T, int64_t _value>
	struct enum_value_to_index : std::integral_constant<int64_t, std::numeric_limits<int64_t>::min()> {};

	template<typename T, int64_t index>
	struct enum_index_to_value : std::integral_constant<int64_t, std::numeric_limits<int64_t>::min()> {};
}

// Enum (base) implementation example
struct enumBase;
template<> struct std::is_enum<enumBase> : std::true_type {};
template<> struct std::is_enum_class<enumBase> : std::true_type {};

template<> struct std::enum_name<enumBase, 0> { constexpr static const char* value = "a"; };
template<> struct std::enum_name<enumBase, 1> { constexpr static const char* value = "b"; };
template<> struct std::enum_name<enumBase, 2> { constexpr static const char* value = "c"; };
template<> struct std::enum_name<enumBase, 3> { constexpr static const char* value = "d"; };

template<> struct std::enum_name_caseless<enumBase, 0> { constexpr static const char* value = "a"; };
template<> struct std::enum_name_caseless<enumBase, 1> { constexpr static const char* value = "b"; };
template<> struct std::enum_name_caseless<enumBase, 2> { constexpr static const char* value = "c"; };
template<> struct std::enum_name_caseless<enumBase, 3> { constexpr static const char* value = "d"; };

template<> struct std::enum_value_to_index<enumBase, 0> : std::integral_constant<int64_t, 0> {};
template<> struct std::enum_value_to_index<enumBase, 1> : std::integral_constant<int64_t, 1> {};
template<> struct std::enum_value_to_index<enumBase, 3> : std::integral_constant<int64_t, 2> {};
template<> struct std::enum_value_to_index<enumBase, 4> : std::integral_constant<int64_t, 3> {};

template<> struct std::enum_index_to_value<enumBase, 0> : std::integral_constant<int64_t, 0> {};
template<> struct std::enum_index_to_value<enumBase, 1> : std::integral_constant<int64_t, 1> {};
template<> struct std::enum_index_to_value<enumBase, 2> : std::integral_constant<int64_t, 3> {};
template<> struct std::enum_index_to_value<enumBase, 3> : std::integral_constant<int64_t, 4> {};

struct enumBase {
	struct detail {
		using enum_base_type = int;
		enum class values : enum_base_type {
			a = 0,
			b,
			c = 3,
			d
		};

		constexpr static const values lastValue = max(values::a, values::b, values::c, values::d);
		constexpr static const size_t lastIndex = 3;
	};

	static const enumBase a; // = detail::values::a;
	static const enumBase b; // = detail::values::b;
	static const enumBase c; // = detail::values::c;
	static const enumBase d; // = detail::values::d;

	detail::values value = (detail::values) 0;

	constexpr explicit enumBase(const detail::values _value = (detail::values) 0) : value(_value) { }
	constexpr explicit operator detail::values () const { return value; }
	constexpr enumBase& operator= (const detail::values _value) { value = _value; return *this;}

	constexpr explicit enumBase(const detail::enum_base_type _value) : value((detail::values) _value) { }
	constexpr explicit operator detail::enum_base_type() const { return (detail::enum_base_type) value; }

	constexpr optional<const char*> to_char_array() const {
		switch(value) {
		case detail::values::a: return std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::a>::value>::value;
		case detail::values::b: return std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::b>::value>::value;
		case detail::values::c: return std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::c>::value>::value;
		case detail::values::d: return std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::d>::value>::value;
		default: return {};
		}
	}
	optional<std::string> tostring() const { if(auto opt = to_char_array(); opt) return *opt; else return {}; }

	constexpr static enumBase from_string(const std::string_view str, const char* type = "enumBase") {
		if(str == std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::a>::value>::value) return (enumBase) detail::values::a;
		else if(str == std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::b>::value>::value) return (enumBase) detail::values::b;
		else if(str == std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::c>::value>::value) return (enumBase) detail::values::c;
		else if(str == std::enum_name<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::d>::value>::value) return (enumBase) detail::values::d;
		else throw std::enum_conversion_error("Failed to convert string `"s + str + "` to " + type);
	}

	/*constexpr*/ static enumBase from_string_caseless(const string::view str, const char* type = "enumBase") {
		if(str.caseless_equal(std::enum_name_caseless<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::a>::value>::value)) return (enumBase) detail::values::a;
		else if(str.caseless_equal(std::enum_name_caseless<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::b>::value>::value)) return (enumBase) detail::values::b;
		else if(str.caseless_equal(std::enum_name_caseless<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::c>::value>::value)) return (enumBase) detail::values::c;
		else if(str.caseless_equal(std::enum_name_caseless<enumBase, std::enum_value_to_index<enumBase, (int64_t) detail::values::d>::value>::value)) return (enumBase) detail::values::d;
		else throw std::enum_conversion_error("Failed to convert string `"s + str + "` to " + type);
	}
};

constexpr const enumBase enumBase::a = (enumBase) enumBase::detail::values::a;
constexpr const enumBase enumBase::b = (enumBase) enumBase::detail::values::b;
constexpr const enumBase enumBase::c = (enumBase) enumBase::detail::values::c;
constexpr const enumBase enumBase::d = (enumBase) enumBase::detail::values::d;

template<> struct std::underlying_type<enumBase> { using type = typename enumBase::detail::enum_base_type; };

std::ostream& operator << (std::ostream& s, const enumBase en) {
	return s << *en.to_char_array();
}

std::odebugstream& operator << (std::odebugstream& s, const enumBase en) {
	auto& _s = reference_cast<std::ostream>(s);
	_s << *en.to_char_array() << " (enum = " << (enumBase::detail::enum_base_type) en.value << ")";
	return s;
}

std::odebugstream& operator << (std::odebugstream& s, const enumBase::detail::values v) {
	return s << (enumBase::detail::enum_base_type) v;
}


// ------------------------


// Enum (Derived) implementation example
struct enumDerive;
template<> struct std::is_enum<enumDerive> : std::true_type {};
template<> struct std::is_enum_class<enumDerive> : std::true_type {};

template<> struct std::enum_name<enumDerive, 0> : public std::enum_name<enumBase, 0> {};
template<> struct std::enum_name<enumDerive, 1> : public std::enum_name<enumBase, 1> {};
template<> struct std::enum_name<enumDerive, 2> : public std::enum_name<enumBase, 2> {};
template<> struct std::enum_name<enumDerive, 3> : public std::enum_name<enumBase, 3> {};
template<> struct std::enum_name<enumDerive, 4> { constexpr static const char* value = "e"; };
template<> struct std::enum_name<enumDerive, 5> { constexpr static const char* value = "F"; };

template<> struct std::enum_name_caseless<enumDerive, 0> : public std::enum_name_caseless<enumBase, 0> {};
template<> struct std::enum_name_caseless<enumDerive, 1> : public std::enum_name_caseless<enumBase, 1> {};
template<> struct std::enum_name_caseless<enumDerive, 2> : public std::enum_name_caseless<enumBase, 2> {};
template<> struct std::enum_name_caseless<enumDerive, 3> : public std::enum_name_caseless<enumBase, 3> {};
template<> struct std::enum_name_caseless<enumDerive, 4> { constexpr static const char* value = "e"; };
template<> struct std::enum_name_caseless<enumDerive, 5> { constexpr static const char* value = "f"; };

template<> struct std::enum_value_to_index<enumDerive, 0> : public std::enum_value_to_index<enumBase, 0> {};
template<> struct std::enum_value_to_index<enumDerive, 1> : public std::enum_value_to_index<enumBase, 1> {};
template<> struct std::enum_value_to_index<enumDerive, 2> : public std::enum_value_to_index<enumBase, 2> {};
template<> struct std::enum_value_to_index<enumDerive, 3> : public std::enum_value_to_index<enumBase, 3> {};
template<> struct std::enum_value_to_index<enumDerive, 4> : public std::enum_value_to_index<enumBase, 4> {};
template<> struct std::enum_value_to_index<enumDerive, (int64_t)enumBase::detail::lastValue + 1> : std::integral_constant<int64_t, 4> {};
template<> struct std::enum_value_to_index<enumDerive, (int64_t)enumBase::detail::lastValue + 2> : std::integral_constant<int64_t, 5> {};

template<> struct std::enum_index_to_value<enumDerive, 0> : public std::enum_index_to_value<enumBase, 0> {};
template<> struct std::enum_index_to_value<enumDerive, 1> : public std::enum_index_to_value<enumBase, 1> {};
template<> struct std::enum_index_to_value<enumDerive, 2> : public std::enum_index_to_value<enumBase, 2> {};
template<> struct std::enum_index_to_value<enumDerive, 3> : public std::enum_index_to_value<enumBase, 3> {};
template<> struct std::enum_index_to_value<enumDerive, 4> : std::integral_constant<int64_t, (int64_t)enumBase::detail::lastValue + 1> {};
template<> struct std::enum_index_to_value<enumDerive, 5> : std::integral_constant<int64_t, (int64_t)enumBase::detail::lastValue + 2> {};

struct enumDerive : enumBase {
private: using Base = enumBase;
public:
	struct detail {
		using enum_base_type = Base::detail::enum_base_type;
		enum class values : enum_base_type {
			e = (enum_base_type) Base::detail::lastValue + 1,
			F
		};

		constexpr static const values lastValue = max((values) Base::detail::lastValue, values::e, values::F);
		constexpr static const size_t lastIndex = 3 + 2;

	};

	static const enumDerive e; // = detail::values::e;
	static const enumDerive F; // = detail::values::F;


	constexpr explicit enumDerive(const detail::values _value = (detail::values) 0) : Base((Base::detail::values) _value) { }
	constexpr operator detail::values () const { return (detail::values) value; }
	constexpr enumDerive& operator= (const detail::values _value) { value = (Base::detail::values)_value; return *this; }

	constexpr enumDerive(const Base::detail::values _value) : Base(_value) { }
	constexpr explicit operator Base::detail::values () const { return (Base::detail::values) value; }
	constexpr enumDerive& operator= (const Base::detail::values _value) { value = _value; return *this; }

	constexpr enumDerive(const Base _value) : Base(_value) { }
	constexpr enumDerive& operator= (const Base _value) { value = (Base::detail::values)_value; return *this; }

	constexpr explicit enumDerive(const detail::enum_base_type _value) : Base((Base::detail::values) _value) { }
	constexpr explicit operator detail::enum_base_type() const { return (detail::enum_base_type) value; }

	constexpr optional<const char*> to_char_array() const {
		switch((detail::values) value) {
		case detail::values::e: return std::enum_name<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::e>::value>::value;
		case detail::values::F: return std::enum_name<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::F>::value>::value;
		default: return Base::to_char_array();
		}
	}
	optional<std::string> tostring() const { if(auto opt = to_char_array(); opt) return *opt; else return {}; }

	constexpr static enumDerive from_string(const std::string_view str, const char* type = "enumDerive") {
		if(str == std::enum_name<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::e>::value>::value) return (enumDerive) detail::values::e;
		else if(str == std::enum_name<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::F>::value>::value) return (enumDerive) detail::values::F;
		else return Base::from_string(str, type);
	}

	/*constexpr*/ static enumDerive from_string_caseless(const string::view str, const char* type = "enumDerive") {
		if(str.caseless_equal(std::enum_name_caseless<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::e>::value>::value)) return (enumDerive) detail::values::e;
		else if(str.caseless_equal(std::enum_name_caseless<enumDerive, std::enum_value_to_index<enumDerive, (int64_t) detail::values::F>::value>::value)) return (enumDerive) detail::values::F;
		else return Base::from_string_caseless(str, type);
	}

	constexpr auto operator<=>(const enumBase b) const {
		return 	a.value == b.value ? std::strong_ordering::equal :
				a.value <  b.value ? std::strong_ordering::less :
			         	 std::strong_ordering::greater;
	}
};

constexpr const enumDerive enumDerive::e = (enumDerive) enumDerive::detail::values::e;
constexpr const enumDerive enumDerive::F = (enumDerive) enumDerive::detail::values::F;

template<> struct std::underlying_type<enumDerive> { using type = typename enumDerive::detail::enum_base_type; };

constexpr auto operator<=>(const enumBase a, const enumDerive b)  { return b <=> a; }
constexpr bool operator==(const enumBase a, const enumDerive b)  { return b.value == a.value; }
constexpr bool operator!=(const enumBase a, const enumDerive b)  { return b.value != a.value; }
constexpr bool operator>(const enumBase a, const enumDerive b)  { return b > a; }
constexpr bool operator<(const enumBase a, const enumDerive b)  { return b < a; }
constexpr bool operator>=(const enumBase a, const enumDerive b)  { return b >= a; }
constexpr bool operator<=(const enumBase a, const enumDerive b)  { return b <= a; }

std::ostream& operator << (std::ostream& s, const enumDerive en) {
	return s << *en.to_char_array();
}

std::odebugstream& operator << (std::odebugstream& s, const enumDerive en) {
	auto& _s = reference_cast<std::ostream>(s);
	_s << *en.to_char_array() << " (enum = " << (enumDerive::detail::enum_base_type) en.value << ")";
	return s;
}

std::odebugstream& operator << (std::odebugstream& s, const enumDerive::detail::values v) {
	return s << (enumDerive::detail::enum_base_type) v;
}


TEST_CASE( "Enum Wrappers" ) {
	// Based on an int... should only be as large as an int
	CHECK( sizeof(enumDerive) == sizeof(int) );

	CHECK( (int)enumBase::detail::lastValue != (int)enumDerive::detail::lastValue );
	CHECK( (int)enumBase::detail::lastValue == 4 );
	CHECK( (int)enumDerive::detail::lastValue == 6 );

	CHECK( enumBase::detail::lastIndex != enumDerive::detail::lastIndex );
	CHECK( enumBase::detail::lastIndex == 3 );
	CHECK( enumDerive::detail::lastIndex == 5);

	CHECK( (int) enumDerive::a == 0 );
	CHECK( enumDerive::b == enumDerive::from_string("b") );
	CHECK( enumDerive::from_string_caseless("f") == enumDerive::F );

	bool exceptionOccured = false;
	try {
		enumDerive::from_string("f");
	} catch (std::enum_conversion_error&) {
		exceptionOccured = true;
	}
	CHECK( exceptionOccured == true );

	std::stringstream capture;
	/* capture(std::cout) */{
		std::streambuf* std__cout_buf = std::cout.rdbuf();
		std::cout.rdbuf(capture.rdbuf());
		defer { std::cout.rdbuf(std__cout_buf); };

		std::cout << enumDerive::a << " - " << enumDerive::F;
	}
	CHECK( capture.str() == "a - F" );

	std::benchmark_header("Enum Lookup");
	BENCHMARK("constexpr") {
		constexpr auto atCompileTime = enumDerive::from_string("F");
		std::do_not_optimize(atCompileTime);
	};

	BENCHMARK("normal") {
		std::do_not_optimize(
			enumDerive::from_string("F")
		);
	};

	BENCHMARK("caseless") {
		std::do_not_optimize(
			enumDerive::from_string_caseless("f")
		);
	};
}

TEST_CASE( "Relative Pointers" ) {
	char here = 'a';
	std::relative_ptr<char, i8> ptr = &here;

	CHECK( &here == ptr.data() );
	CHECK( sizeof(ptr) == sizeof(i8) );

	CHECK( *(&here) == *ptr );
}

TEST_CASE( "Based Pointers" ) {
	char here = 'a';
	std::based_ptr<char, i8> ptr = &here;

	CHECK( &here == ptr.data() );
	CHECK( sizeof(ptr) == std::bit_ceil(sizeof(i8) + sizeof(void*)) );

	CHECK( *(&here) == *ptr );

	ptr.set_base(&here - 20);
	CHECK( &here == ptr.data() );
}

TEST_CASE( "Nonnull Pointers" ) {
	char here = 'a';
	std::not_null<char*> ptr = &here;

	CHECK( &here == ptr );
	CHECK( *(&here) == *ptr );

	bool except = false;
	try {
		char* n = nullptr;
		ptr = n;
	} catch(std::invalid_argument&) {
		except = true;
	}
	CHECK( except == true);
}

TEST_CASE( "Observable Pointers" ) {
	// Non-owning pointer that will outlive the object
    std::observer_ptr<std::string> obs_ptr;

    {
        // Sealed (unique) pointer that owns the object
        auto owner_ptr = std::make_observable_sealed<std::string>("hello");

        // A sealed pointer cannot be copied but it can be moved
        // auto tmp_copied = owner_ptr; // error!
        // auto tmp_moved = std::move(owner_ptr); // OK

        // Make the observer pointer point to the object
        obs_ptr = owner_ptr;

        // The observer pointer is now valid
        CHECK(obs_ptr.expired() == false);

        // It can be used like a regular raw pointer
        CHECK(obs_ptr != nullptr);
        std::cout << *obs_ptr << std::endl;

        // An observer pointer can be copied and moved
        // auto tmp_copied = obs_ptr; // OK
        // auto tmp_moved = std::move(obs_ptr); // OK
    }

    // The sealed pointer has gone out of scope, the object is deleted,
    // the observer pointer is now null.
    CHECK(obs_ptr.expired() == true);
    CHECK(obs_ptr == nullptr);
}

// template <class T, class ExtsA, class LayA, class AccA, class ExtsB, class LayB, class AccB >
// T dot_product(std::mdarray<T, ExtsA, LayA, AccA> const& a, std::mdarray<T, ExtsB, LayB, AccB> const& b)
// 	requires (ExtsA::rank() == ExtsB::rank() && ExtsA::rank() == 2)
// {
// 	T result = 0;
// 	for(int i = 0; i < a.extent(0); ++i)
// 		for(int j = 0; j < a.extent(1); ++j)
// 			result += a(i, j) * b(i, j);
// 	return result;
// }

// //================================================================================

// template <class T, class ExtsA, class LayA, class AccA>
// void fill_in_order(std::mdarray<T, ExtsA, LayA, AccA>& a)
// 	requires (ExtsA::rank() == 2)
// {
// 	T count = 0;
// 	for(int i = 0; i < a.extent(0); ++i)
// 		for(int j = 0; j < a.extent(1); ++j)
// 			a(i, j) = count++;
// }

// TEST_CASE( "Multiarray" ) {
// 	mdarray<int, 10, 10, 2> a;

// 	constexpr int rows = 3;
// 	constexpr int cols = 3;

// 	{
// 		using array_2d_dynamic = std::mdarray<int, std::extents<std::dynamic_extent, std::dynamic_extent>, std::layout_right>;
// 		using array_2d_dynamic_left = std::mdarray<int, std::extents<std::dynamic_extent, std::dynamic_extent>, std::layout_left>;

// 		auto a = array_2d_dynamic(rows, cols);
// 		auto b = array_2d_dynamic_left(rows, cols);

// 		fill_in_order(a);
// 		fill_in_order(b);

// 		CHECK(dot_product(a, b) == 204);
// 	}
// }

// #include <coroutine>

// TEST_CASE( "Generator" ) {

// 	auto iota = [](int start = 0, int end = -1) -> std::coroutine::generator<int> {
// 		for( ; start < end || end < start; start++)
// 			co_yield start;
// 	};

// 	int last = 0;
// 	for(int x: iota(0, 10))
// 		CHECK(x == last++);
// }

// TEST_CASE( "Variant" ) {
// 	using variant = dl::variant<int, string, float>;
// 	variant v;
// 	v = "bob";
// 	v = 7;

// 	CHECK( v.holds_type<int>() == true );
// 	CHECK( v.holds_value(7) == true );
// 	CHECK( *v.value<int>() == 7 );
// 	CHECK( v.value<string>() == std::nullopt );

// 	any a;
// 	a = (string) "bob";

// 	CHECK( a.holds_type<string>() == true );
// 	CHECK( a.holds_value<string>("bob") == true );
// 	CHECK( *a.value<string>() == "bob" );
// 	CHECK( a.value_or<int>(0) == 0 );

// 	v = a.variant<int, string, float>();

// 	CHECK( v.holds_type<string>() == true );
// 	CHECK( v.holds_value<string>("bob") == true );
// 	CHECK( *v.value<string>() == "bob" );
// 	CHECK( v.value<int>() == std::nullopt );

// 	a = 7;
// 	v = a.variant(variant::types{});

// 	CHECK( v.holds_type<int>() == true );
// 	CHECK( v.holds_value(7) == true );
// 	CHECK( *v.value<int>() == 7 );
// 	CHECK( v.value<string>() == std::nullopt );

// 	expected<int, string> e = 7;
// 	// optional<int> i = v.value<int>();

// 	CHECK( e.has_value() == true );
// 	CHECK( e.value() == 7 );

// 	e = std::make_unexpected<string>("An error occured!");

// 	CHECK( e.has_value() == false );
// 	CHECK( e.value_or(0) == 0 );
// 	CHECK( e.error() == "An error occured!" );
// }


#include <delegate>

struct Hello  {
	void operator()() const {
		std::cout << "Hello";
	}
};

void World() {
	std::cout << " World!" << std::endl;
}

struct MaximumBehavior {
	template<class Itterator>
	auto operator()(Itterator begin, Itterator end) {
		typename Itterator::value_type max = -INFINITY;
		for(auto it = begin; it != end; it++)
			max = std::max(max, *it);
		return max;
	}
};

struct MinimumBehavior {
	template<class Itterator>
	auto operator()(Itterator begin, Itterator end) {
		typename Itterator::value_type min = INFINITY;
		for(auto it = begin; it != end; it++)
			min = std::min(min, *it);
		return min;
	}
};

TEST_CASE( "Delegate" ) {
	std::delegate<void()> helloWorld;

	Hello h;
	helloWorld += h;
	helloWorld += World;

	{CAPTURE_CONSOLE
			helloWorld();
		}

		CHECK(capture.str() == "Hello World!\n");
	}

	helloWorld -= World;

	{CAPTURE_CONSOLE
			helloWorld();
		}

		CHECK(capture.str() == "Hello");
	}


	std::delegate<float(float, float)> s;

	// Sum
	s += std::make_delegate([](float a , float b) -> float {
		return a + b;
	});
	// Difference
	s += std::make_delegate([](float a , float b) -> float {
		return a - b;
	});
	// Product
	auto productDelegate = std::make_delegate([](float a , float b) -> float {
		return a * b;
	});
	s += productDelegate;
	// Quotient (closure)
	s.connect( std::make_delegate([&](float a , float b) -> float {
		return a / b;
	}) );

	// Convertable delegate
	s += std::make_delegate([](int a, int b) {
		return a + b;
	});

	// Empty delegate
	auto emptyDelegate = std::make_delegate([]{
		return -5.f;
	});
	s += emptyDelegate;

	CHECK( s(MaximumBehavior{}, 3, 5) == 15 );

	s -= productDelegate;

	CHECK( s(MaximumBehavior{}, 3, 5) == 8 );
	CHECK( s(MinimumBehavior{}, 3, 5) == -5 );

	s.disconnect(emptyDelegate);

	CHECK( s(MinimumBehavior{}, 3, 5) == -2 );

	std::benchmark_header();
	BENCHMARK("Math") {
		std::do_not_optimize(
			s(MaximumBehavior{}, 3, 5)
		);
	};
}

// TEST_CASE( "Binary Transformation" ) {
// 	float f = 5;
// 	double d = 7;
// 	string str = "bob";
// 	dl::vector<int> v = {1, 2, 3, 4, 5, 6, 7};

// 	std::stringstream s;
// 	s << std::binary(f).convert_endian(std::endian::little)
// 		<< std::binary(f).convert_big_endian()
// 		<< std::binary(d)
// 		<< std::binary(str)
// 		<< std::binary(v).convert_big_endian();

// 	// for(char b: s.str())
// 	// 	std::cout << (std::byte) b << " ";
// 	// std::cout << std::endl;

// 	std::stringstream sin(s.str());
// 	f = 0;
// 	d = 0;
// 	str.clear();
// 	v.clear();
// 	float f2;
// 	sin >> std::binary{f}//.convert_big_endian()
// 		>> std::binary{f2}.convert_big_endian()
// 		>> std::binary{d}
// 		>> std::binary{str}
// 		>> std::binary{v}.convert_endian(std::endian::big);

// 	CHECK(f == 5);
// 	CHECK(f2 == 5); // TODO:
// 	CHECK(d == 7);
// 	CHECK(str == "bob");
// 	for(int i = 1; i <= 7; i++)
// 		CHECK(v[i - 1] == i);
// }

// #include <io>

// TEST_CASE( "fast_io" ) {
// 	size_t a = 5, b = 6;

// 	std::println("Hello world", "\n");
// 	std::print << (a + b) << std::io::endl;

// 	std::debug::print << (a + b) << std::io::endl;
// 	std::perr << "This line is an errror" << std::io::endl;

// 	std::benchmark_header("IO");
// 	BENCHMARK("fast_io") {
// 		perr(a + b);
// 		perrln(a + b);
// 	};

// 	BENCHMARK("fast_io_stream") {
// 		std::perr(a + b);
// 		std::perr << (a + b) << std::io::endl;
// 	};

// 	BENCHMARK("iostream") {
// 		std::cerr << (a + b) << std::flush;
// 		std::cerr << (a + b) << std::__1::endl;
// 	};
// }

#include <math>

TEST_CASE( "Math" ) {
	CHECK(std::lerp(0., 5, .5) == 2.5);
	CHECK(std::smoothstep(0., 5., .5) == std::approx(0.028));

	CHECK(std::wrap(0.0, 360., 719.99) == 359.99);
	CHECK(std::wrap(0.0, 360., 720.1) == std::approx(.1));
	CHECK(std::wrap(0, 360, 720) == 0);
	CHECK(std::wrap(10, 15, 21) == 11);

	CHECK(std::map_unsigned(50) == 100);
	CHECK(std::map_unsigned(-50) == 99);
	CHECK(std::unmap_unsigned(100u) == 50);
	CHECK(std::unmap_unsigned(99u) == -50);

	CHECK(std::numeric_pair(50u, 60u) == 6165);
	CHECK(std::numeric_pair(std::map_unsigned(-50), std::map_unsigned(-60)) == 23990);
}

enum class MyEnum { Value1 = 1 << 0, Value2 = 1 << 1, Value3 = 1 << 2 };
FLAG_OPS(MyEnum)

TEST_CASE( "Flags" ) {
	auto mask1 = MyEnum::Value1 | MyEnum::Value2; // set flags Value1 and Value 2
	CHECK( (mask1 & MyEnum::Value1) > 0 );
	CHECK( (mask1 & MyEnum::Value2) > 0 );
	CHECK( (mask1 & MyEnum::Value3) == 0 );

	auto mask2 = MyEnum::Value3 | MyEnum::Value2;
	CHECK( (mask2 & MyEnum::Value1) == 0 );
	CHECK( (mask2 & MyEnum::Value2) > 0 );
	CHECK( (mask2 & MyEnum::Value3) > 0 );

	mask2 = mask1;
	CHECK( (mask2 & MyEnum::Value1) > 0 );
	CHECK( (mask2 & MyEnum::Value2) > 0 );
	CHECK( (mask2 & MyEnum::Value3) == 0 );
}