#include <benchmark/benchmark.h>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"

static void BM_LexerTokenization(benchmark::State& state) {
    std::string input = R"(\int_{\alpha}^{\beta} f'(x) \, dx = f(\beta) - f(\alpha))";

    for (auto _ : state) {
        Lexer math_lexer(input);
        auto tokens = math_lexer.tokenize();

        benchmark::DoNotOptimize(tokens);
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
