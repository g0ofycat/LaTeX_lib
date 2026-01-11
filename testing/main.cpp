#include <iostream>
#include <vector>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"

int main() {
    Lexer math_lexer("42");

    auto tokens = math_lexer.tokenize();

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
