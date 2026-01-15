#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string equation = R"(\sqrt{-1})";

    for (auto _ : state)
    {
        LatexCore core_impl(equation);

        benchmark::DoNotOptimize(core_impl);
    }

    LatexCore core_impl(equation);

    core_impl.print_analysis();
    core_impl.print_tokens();
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
