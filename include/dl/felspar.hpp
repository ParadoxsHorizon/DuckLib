#ifndef __DUCKED_C_FELSPAR__
#define __DUCKED_C_FELSPAR__

#include "felspar/coro/always.hpp"
#include "felspar/coro/cancellable.hpp"
#include "felspar/coro/generator.hpp"
#include "felspar/coro/lazy.hpp"
#include "felspar/coro/start.hpp"
#include "felspar/coro/stream.hpp"
#include "felspar/coro/task.hpp"
#include "felspar/coro/to_stream.hpp"

#include <felspar/exceptions.hpp>
#include <felspar/memory/small_vector.hpp>

namespace std {
	inline namespace coroutine {
		using namespace felspar::coro;
	}

	namespace annotated {
		using felspar::exceptions::source_annotation;
		using namespace felspar::stdexcept;
	}

	// TODO: Should small vector be named something else? static_vector? fixed_vector? stack_vector?
	using felspar::memory::small_vector;
}

#endif // __DUCKED_C_FELSPAR__