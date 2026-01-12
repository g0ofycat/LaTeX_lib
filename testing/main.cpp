#include <iostream>
#include <vector>
#include <chrono>

#include "../src/lexer/lexer.hpp"

#include "../src/parser/parser.hpp"

int main() {
    Lexer math_lexer(R"(\int_{\alpha}^{\beta} f'(x) \, dx = f(\beta) - f(\alpha))");

    auto start = std::chrono::high_resolution_clock::now();
    auto tokens = math_lexer.tokenize();
    auto end = std::chrono::high_resolution_clock::now();

    double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Tokenization Speed: " << elapsed_ms << " ms\n";

    std::cout << "Tokens (" << tokens.size() << "):\n";

    for (const auto& tok : tokens) {
        std::cout << "  "
                  <<  static_cast<int>(tok.Type)
                  << "  \"" << tok.Value << "\""
                  << "  @" << tok.line << ":" << tok.column
                  << "\n";
    }

    return 0;
}
