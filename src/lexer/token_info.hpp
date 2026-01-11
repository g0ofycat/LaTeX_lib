#ifndef TOKEN_INFO_HPP
#define TOKEN_INFO_HPP

#include <string_view>

#include "../parser/data/latex_info.hpp"

// ======================
// -- ENUMS / STRUCTS
// ======================

enum class TokenType {
    NUMBER, COMMAND, BRACE_OPEN, BRACE_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, PLUS, MINUS, STAR, SLASH, IDENTIFIER, SUBSCRIPT, SUPERSCRIPT, DOLLAR, ALIGNMENT, WHITESPACE, END_OF_FILE, INVALID, UNKNOWN
};

struct Token {
    std::string_view Value; // std::string_view: Value of the string
    const CommandInfo* Info = nullptr; // CommandInfo: Metadata for each token
    TokenType Type; // TokenType: Type of token
    int line; // int: Current line
    int column; // int: Current column
};

#endif
