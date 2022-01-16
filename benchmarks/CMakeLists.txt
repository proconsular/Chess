set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp board_benchmark.cpp smart_ai_player_benchmark.cpp)
include_directories(../include/benchmark)

add_executable(benchmark ${SOURCE_FILES})
target_link_libraries(benchmark ${PROJECT_SOURCE_DIR}/lib/benchmark/libbenchmark.a ${PROJECT_SOURCE_DIR}/lib/benchmark/libbenchmark_main.a source)