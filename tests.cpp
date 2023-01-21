#define TESTING_DEFINE_MAIN // generate a main function to run the tests
#include <ducklib>
#include <testing>
#include <stacktrace>
#include <math>

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
	CHECK( std::demangle(typeid(std::stacktrace)) == "std::basic_stacktrace<std::allocator<std::stacktrace_entry> >");
	CHECK( std::demangle(typeid(std::cout)) == "std::ostream");
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
	CHECK( sizeof(f16)  == 16 / 8 );
	CHECK( sizeof(f32)  == 32 / 8 );
	CHECK( sizeof(f64)  == 64 / 8 );
	CHECK( sizeof(f128) == 128 / 8 + 16 ); // Extended types are documented to be a word larger than they should be
	CHECK( sizeof(f256) == 256 / 8 + 32 ); // Extended types are documented to be a word larger than they should be // TODO: Why is this two words longer?

	CHECK( f16::min == std::approx(6.10352e-05) );
	CHECK( f16::max == 65504 );
	CHECK( i8::min == -128 );
	CHECK( i8::max == 127 );
	CHECK( u8::min == 0 );
	CHECK( u8::max == 255 );
	// CHECK( i1024::min == -0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFi1024r );
	// CHECK( i1024::max == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFi1024 );
	CHECK( u1024::min == 0 );
	// CHECK( u1024::max == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFu1024r );
	CHECK( std::isnan(f32::nan) == true );

	CHECK( std::numeric_limits<i8>::is_specialized == true );
	CHECK( std::numeric_limits<i8>::min() == -128 );

	rational rat = 7;
	rat /= 3;
	rat *= 2;
	CHECK( double(rat) == 14.0 / 3 );

	iarb inf = 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024; inf *= 1024;
	// CHECK( inf == 0x400000000000000000000000000000000000000000000000000000000000000i1024r );

	// std::cout << inf << std::endl;
	// std::cout << 0xC05748BBFB5ACD7E5A77DC03D9EC7D8BB957C1B95D9B206090D83FD1B67433CE83EAD7376CCFD612C72901F4CE0A2E07E322D438EA4F34647555D62D04140E1084E999BB4CD5F947A76674009E2318549FD102C5F7596EDC332A0DDEE3A355186B9A046F0F96A279C1448A9151549DC663DA8A6E89CF8F511BAED6450DA2C1CBi1024 << std::endl;
	// std::cout << sizeof(iarb) << std::endl;
}

TEST_CASE("String Extensions") {
	// Fails due to minor inprecision
	string n = "127.889";
	CAPTURE_CONSOLE
		std::cout << float(n);
	}
	CHECK( capture.str() == n );

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
	CHECK( s[slice(0, 4)] == "Hell"sv);
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

	s = "20";
	CHECK( s.natural_less("2") == true );
	CHECK( s.natural_greater("3") == false );
	CHECK( s.natural_greater("30") == true );
	CHECK( s.natural_equal("20") == true );
	CHECK( s.natural_equal("30") == false );

	string::view sv = "こんにちは世界"sv;
	auto u8iter = sv.begin(std::utf8);
	CHECK( string::view(*(++u8iter)) == "ん"sv );

	dynarray<string> values = {"this", "is", "a", "comma", "seperated", "list"};
	s = ", "s.join(values);
	CHECK( s == "this, is, a, comma, seperated, list");
}

TEST_CASE( "Array" ) {
	array<int, 3> a = {1, 2, 3}, b = {4, 5, 6};

	auto add = a + b;
	for(int i = 0; i < 6; i++)
		CHECK( add[i] == i + 1 );

	auto sub = add[slice(2, 4)];
	CHECK( sub.size() == 2 );
	CHECK( sub.front() == 3 );
	CHECK( sub.back() == 4 );

	auto inclusiveSub = add[slice_inclusive(2, 4)];
	CHECK( inclusiveSub.size() == 3 );
	CHECK( inclusiveSub.front() == 3 );
	CHECK( inclusiveSub.back() == 5 );
	
	CHECK( add.contains(3) == true );
	CHECK( add.contains(5) == true );

	array<int, 5> fives = 5;
	CHECK(fives.size() == 5);
	for(int cur: fives)
		CHECK(cur == 5);
}

TEST_CASE( "Dynarray" ) {
	dynarray<int> a = {1, 2, 3}, b = {4, 5, 6};

	auto add = a + b;
	for(int i = 0; i < 6; i++)
		CHECK( add[i] == i + 1 );

	auto sub = add[slice(2, 4)];
	CHECK( sub.size() == 2 );
	CHECK( sub.front() == 3 );
	CHECK( sub.back() == 4 );

	auto inclusiveSub = add[slice_inclusive(2, 4)];
	CHECK( inclusiveSub.size() == 3 );
	CHECK( inclusiveSub.front() == 3 );
	CHECK( inclusiveSub.back() == 5 );
	
	CHECK( add.contains(3) == true );

	add.at(7) = 7;
	CHECK( add.back() == 7 );

	auto repeated = add * 3;
	CHECK(repeated.size() == 3 * 8);
	CHECK(std::count(repeated.begin(), repeated.end(), 3) == 3);

	repeated *= 3;
	CHECK(repeated.size() == 3 * 8 * 3);
	CHECK(std::count(repeated.begin(), repeated.end(), 3) == 3 * 3);

	a += b;
	for(int i = 0; i < 6; i++)
		CHECK( a[i] == i + 1 );
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
	CHECK( std::swap_endian((u64) 0x65) 		      == 0x6500000000000000);

	// benchmarks
	std::benchmark::header("Endians");
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


// --- Variant & Any ---


TEST_CASE( "Variant & Any" ) {
	using variant = dl::variant<int, string, float>;
	variant v;
	v = "bob";
	v = 7;

	CHECK( v.holds_type<int>() == true );
	CHECK( v.holds_value(7) == true );
	CHECK( *v.value<int>() == 7 );
	CHECK( v.value<string>() == std::nullopt );

	any a;
	a = (string) "bob";

	CHECK( a.holds_type<string>() == true );
	CHECK( a.holds_value<string>("bob") == true );
	CHECK( *a.value<string>() == "bob" );
	CHECK( a.value_or<int>(0) == 0 );

	v = a.variant<int, string, float>();

	CHECK( v.holds_type<string>() == true );
	CHECK( v.holds_value<string>("bob") == true );
	CHECK( *v.value<string>() == "bob" );
	CHECK( v.value<int>() == std::nullopt );

	a = 7;
	v = a.variant(variant::types{});

	CHECK( v.holds_type<int>() == true );
	CHECK( v.holds_value(7) == true );
	CHECK( *v.value<int>() == 7 );
	CHECK( v.value<string>() == std::nullopt );

	dl::variant<int, string, float, bool> v2 = v;
	CHECK( v2.holds_type<int>() == true );
	CHECK( v2.holds_value(7) == true );
	CHECK( *v2.value<int>() == 7 );
}

TEST_CASE( "Variant of Variants" ) {
	struct Identifier {};

	using LiteralVariant = std::variant<string, rational, bool>;
	using IdentififerLiteralVariant = std::variant<Identifier, string, rational, bool>;

	std::variant<Identifier, LiteralVariant> v;
	if(v.index() == 0) {
		std::do_not_optimize( std::get<Identifier>(v) );
		CHECK(true == true);
	} else {
		IdentififerLiteralVariant out;
		std::visit([&out](auto& val) { out = val; }, std::get<LiteralVariant>(v));
		std::do_not_optimize(out);
		CHECK(false == true);
	}
}

TEST_CASE( "Expected" ) {
	expected<int, string> e = 7;
	// optional<int> i = v.value<int>();

	CHECK( e.has_value() == true );
	CHECK( e.value() == 7 );

	e = std::make_unexpected<string>("An error occurred!");

	CHECK( e.has_value() == false );
	CHECK( e.value_or(0) == 0 );
	CHECK( e.error() == "An error occurred!" );
}


// ---- Math ----


struct S {
	int a;
	double b;
};

TEST_CASE( "Binary Transformation" ) {
	float f = 5;
	double d = 7;
	string str = "bob";
	S struc = {5, 6};
	dynarray<int> v = {1, 2, 3, 4, 5, 6, 7};

	std::stringstream s;
	s << std::binary(f).convert_endian(std::endian::little)
		<< std::binary(f).convert_big_endian()
		<< std::binary(d)
		<< std::binary(str)
		<< std::binary(struc);
		// << std::binary(v).convert_big_endian();

	// for(char b: s.str())
	// 	std::cout << (std::byte) b << " ";
	// std::cout << std::endl;

	std::stringstream sin(s.str());
	f = 0;
	d = 0;
	str.clear();
	v.clear();
	S s2;
	float f2;
	sin >> std::binary{f}//.convert_little_endian()
		>> std::binary{f2}.convert_big_endian()
		>> std::binary{d}
		>> std::binary{str}
		>> std::binary{s2};
		// >> std::binary{v}.convert_endian(std::endian::big);

	CHECK(f == 5);
	CHECK(f2 == 5); // TODO:
	CHECK(d == 7);
	CHECK(str == "bob");
	CHECK(s2.a == 5);
	CHECK(s2.b == 6);
	// for(int i = 1; i <= 7; i++)
	// 	CHECK(v[i - 1] == i);
}


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