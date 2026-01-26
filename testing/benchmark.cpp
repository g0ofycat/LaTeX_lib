#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string equation = R"(f _ { 9 } ^ { A } = V _ { 1 1 } \sum _ { ( n ^ { 1 } , n ^ { 2 } ) \ne ( 0 , 0 ) } \left( n ^ { I } g _ { I J } n ^ { J } \right) ^ { - 3 / 2 } \ , \quad V _ { 1 1 } = \sqrt { \det g _ { I J } })";

    for (auto _ : state)
    {
        LatexCore core_impl(equation);

        benchmark::DoNotOptimize(core_impl);
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
