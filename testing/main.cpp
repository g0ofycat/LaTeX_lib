#include <iostream>
#include <vector>

#include "../src/lexer/lexer.hpp"

int main() {
    Lexer lex("f{x} + \\frac{f{a}}{1} + \\frac{f{a}}{2}");

    std::vector<Token> tokens = lex.tokenize();

    for (const auto& token : tokens) {
        if (token.Type == TokenType::END_OF_FILE) break;

        std::cout << "Token: [" << token.Value << "]";

        if (token.Info)
            std::cout << " (Metadata: " << token.Info->LaTeX << ")";

        std::cout << std::endl;
    }

    return 0;
} 
