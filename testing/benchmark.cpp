#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string equation = R"(\mathcal{F}\{f\}(\xi) = \int_{-\infty}^{\infty} f(x)e^{-2\pi i x \xi} dx)";

    for (auto _ : state)
    {
        LatexCore core_impl(equation);

        benchmark::DoNotOptimize(core_impl);
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
