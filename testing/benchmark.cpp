#include <iostream>
#include <benchmark/benchmark.h>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"
#include "../src/sem_analyzer/semantic_analyzer.hpp"

static void BM_LexerTokenization(benchmark::State& state) {
    std::string input = R"(\sqrt{-1})";

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

    Parser parser(tokens);
    auto ast = parser.parse();

    SemanticAnalyzer analyzer;
    analyzer.analyze(ast);

    std::cout << "Errors (" << analyzer.get_errors().size() << "):";

    for (const auto& error : analyzer.get_errors()) {
        std::cout << "  "
                  <<  error.message
                  << "  @" << error.line << ":" << error.column
                  << "\n";
    }
}

BENCHMARK(BM_LexerTokenization);

BENCHMARK_MAIN();
