#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string equation = R"(\sum_{k=1}^{\infty} \frac{1}{k^2} = \frac{\pi^2}{6})";

    for (auto _ : state)
    {
        LatexCore core_impl(equation);

        benchmark::DoNotOptimize(core_impl);
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
