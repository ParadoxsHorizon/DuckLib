#define TESTING_DEFINE_MAIN // generate a main function to run the tests
#include <ducklib>
#include <testing>
#include <stacktrace>

using namespace dl::builtin;
using namespace dl::literals;

// Captures the console output (contains an open block)
#define CAPTURE_CONSOLE std::stringstream capture; \
/* capture(std::cout) */{\
	std::streambuf* std__cout_buf = std::cout.rdbuf();\
	std::cout.rdbuf(capture.rdbuf());\
	defer { std::cout.rdbuf(std__cout_buf); };\



#include <pointer>

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

	// Convertible delegate
	s += std::make_delegate([](int a, int b) {
		return a + b;
	});

	// Empty delegate
	auto emptyDelegate = std::make_delegate([]{
		return -5.f;
	});
	s += emptyDelegate;

	CHECK( s(MaximumBehavior{}, 3, 5) == 15 );

	s -= productDelegate; // TODO: Removal isn't working!

	CHECK( s(MaximumBehavior{}, 3, 5) == 8 );
	CHECK( s(MinimumBehavior{}, 3, 5) == -5 );

	s.disconnect(emptyDelegate);

	CHECK( s(MinimumBehavior{}, 3, 5) == -2 );

	std::benchmark::header();
	BENCHMARK("Math") {
		std::do_not_optimize(
			s(MaximumBehavior{}, 3, 5)
		);
	};
}

TEST_CASE( "ValueDelegate" ) {
	std::value_delegate<int> i = 0;
	bool first = true;
	i.on_change() = [&first](int old, int new_) {
		// std::cout << old << " -> " << new_ << std::endl;
		if(first) {
			CHECK(old == 0);
			CHECK(new_ == 5);
			first = false;
		} else {
			CHECK(old == 5);
			CHECK(new_ == 27);
		}
	};

	i = 5;
	CHECK( i == 5 );
	i = 27;
	CHECK( i == 27 );

	std::value_delegate<array<int, 2>> a = array<int, 2>{7, 4};
	a.on_change() = [](auto& old, auto& new_) {
		// std::cout << old[0] << " -> " << new_[0] << std::endl;
		CHECK( (old)[0] == 7 );
		CHECK( (old)[1] == 4 );
		CHECK( (new_)[0] == 4 );
		CHECK( (new_)[1] == 7 );
	};

	{
		auto r = a.record_changes();
		std::ranges::sort(*r);
	}
	CHECK( (*a)[0] == 4 );
	CHECK( (*a)[1] == 7 );
}