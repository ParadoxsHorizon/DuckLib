#include "stream"

namespace std {
	// Memory backings for the debug streams
	odebugstream cdbg = odebugstream(cerr);
	wodebugstream wcdbg = wodebugstream(wcerr);
}
