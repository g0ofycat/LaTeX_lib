#include <iostream>
#include <benchmark/benchmark.h>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"

static void BM_LexerTokenization(benchmark::State& state) {
    std::string input = R"(\int_{\alpha}^{\beta} f'(x) \, dx = f(\beta) - f(\alpha))";

    for (auto _ : state) {
        Lexer benchmark_lexer(input);
        auto tokens = benchmark_lexer.tokenize();

        benchmark::DoNotOptimize(tokens);
    }

    Lexer output_lexer(input);

    auto tokens = output_lexer.tokenize();

    std::cout << "Tokens (" << tokens.size() << "):\n";

    for (const auto& tok : tokens) {
        std::cout << "  "
                  <<  static_cast<int>(tok.Type)
                  << "  \"" << tok.Value << "\""
                  << "  @" << tok.line << ":" << tok.column
                  << "\n";
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
