// Include the benchmarking implementation code
#define ANKERL_NANOBENCH_IMPLEMENT
#define DOCTEST_CONFIG_IMPLEMENT
#include <testing>

// Memory backing for string used to set the category of each benchmark
std::string dl::benchmarkTitle = "benchmark";