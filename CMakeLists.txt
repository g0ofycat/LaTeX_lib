cmake_minimum_required(VERSION 3.16)
project(LaTeX_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(benchmark CONFIG REQUIRED)

add_executable(main 
    testing/main.cpp 
    src/lexer/lexer_registry.cpp 
    src/parser/data/latex_registry.cpp
    src/parser/data/latex_commands_data.cpp
    src/parser/parser_registry.cpp
    src/ast/node_registry.cpp
)

target_include_directories(main PRIVATE 
    src/lexer
    src/parser/data
)

target_link_libraries(main PRIVATE benchmark::benchmark)

target_link_options(main PRIVATE
    -static-libgcc
    -static-libstdc++
)

