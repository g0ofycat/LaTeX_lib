cmake_minimum_required(VERSION 3.16)
project(LaTeX_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(benchmark CONFIG REQUIRED)

add_executable(main
    testing/benchmark.cpp
    src/lexer/lexer_registry.cpp
    src/parser/data/latex_registry.cpp
    src/parser/data/latex_commands_data.cpp
    src/parser/parser_registry.cpp
    src/ast/node_registry.cpp
    src/sem_analyzer/semantic_analyzer_registry.cpp
    src/sem_analyzer/semantic_dispatch_table.cpp
    src/core/core_registry.cpp
)

target_compile_options(main PRIVATE -fverbose-asm -save-temps=obj)

target_include_directories(main PRIVATE 
    src/lexer
    src/parser/data
    src/parser
    src/ast
    src/sem_analyzer
)

target_link_libraries(main PRIVATE benchmark::benchmark shlwapi)

add_definitions(-DBENCHMARK_STATIC_DEFINE)

target_link_options(main PRIVATE
    -static-libgcc
    -static-libstdc++
)

add_executable(bench
    testing/benchmark.cpp
    src/lexer/lexer_registry.cpp
    src/parser/data/latex_registry.cpp
    src/parser/data/latex_commands_data.cpp
    src/parser/parser_registry.cpp
    src/ast/node_registry.cpp
    src/sem_analyzer/semantic_analyzer_registry.cpp
    src/sem_analyzer/semantic_dispatch_table.cpp
    src/core/core_registry.cpp
)

target_include_directories(bench PRIVATE 
    src/lexer
    src/parser/data
    src/parser
    src/ast
    src/sem_analyzer
)

target_link_libraries(bench PRIVATE
    benchmark::benchmark
    benchmark::benchmark_main
    shlwapi
)

target_link_options(bench PRIVATE -static-libgcc -static-libstdc++)
