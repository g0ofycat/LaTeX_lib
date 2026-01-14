#include <string>
#include <vector>

#include "./latex_core.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

// ======================
// -- INIT
// ======================

/// @brief Process a string and return analysis results
/// @param text: The text to process
/// @return std::vector<SemanticError>
std::vector<SemanticError> LatexCore::analyze(const std::string &text)
{
    Lexer lexer(text);

    Parser parser(lexer.tokenize());

    SemanticAnalyzer analyzer;
    analyzer.analyze(parser.parse());

    return analyzer.get_errors();
}