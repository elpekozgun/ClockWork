#include <chrono>

#define TIMEIT(code, message) \
    do { \
        auto start_time = std::chrono::high_resolution_clock::now(); \
        code; \
        auto end_time = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> elapsed_time = end_time - start_time; \
		auto ns = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time);\
        std::cout << message << ": "<< ns.count() << " ms\n"; \
    } while (0)