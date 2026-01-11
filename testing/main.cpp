#include <iostream>
#include <vector>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"

int main() {
    Lexer lex("f{x} + \\frac{f{a}}{1} + \\frac{f{a}}{2}");
    Parser parser(lex.tokenize());
 
    std::unique_ptr<ASTNode> ParsedString = parser.parse(); 

    return 0;
} 
