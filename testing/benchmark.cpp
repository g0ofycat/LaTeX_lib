#include <iostream>
#include <benchmark/benchmark.h>

#include "../core/latex_core.hpp"

static void BM_LexerTokenization(benchmark::State &state)
{
    std::string input = R"(x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a} + \sqrt[n]{\frac{x^n + y^n}{1 + \frac{1}{x^2}}})";

    for (auto _ : state)
    {
        LatexCore::analyze(input);

        benchmark::DoNotOptimize(input);
    }

    for (const auto &error : LatexCore::analyze(input))
    {
        std::cout << "  "
                  << error.message
                  << "  @" << error.line << ":" << error.column
                  << "\n";
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
