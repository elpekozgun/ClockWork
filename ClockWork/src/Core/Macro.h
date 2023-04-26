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


#define ProfileGpu(Code, message) \
    do { \
        GLuint query[2]; \
        glGenQueries(2, query); \
        glQueryCounter(query[0], GL_TIMESTAMP); \
        Code \
        glQueryCounter(query[1], GL_TIMESTAMP); \
        GLuint64 start_time, end_time; \
        glGetQueryObjectui64v(query[0], GL_QUERY_RESULT, &start_time); \
        glGetQueryObjectui64v(query[1], GL_QUERY_RESULT, &end_time); \
        GLuint64 elapsed_time = end_time - start_time; \
        std::cout << message << ": " <<  elapsed_time / 1000000.0 << " milliseconds" << std::endl; \
        glDeleteQueries(2, query); \
    } while(0)