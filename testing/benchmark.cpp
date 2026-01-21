#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string equation = R"(\[\nx = \frac{-b \pm \sqrt{b^2 -4ac}}{2a}\])";

    for (auto _ : state)
    {
        LatexCore core_impl(equation);

        benchmark::DoNotOptimize(core_impl);
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
