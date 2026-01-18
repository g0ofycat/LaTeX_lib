#ifndef TOKEN_INFO_HPP
#define TOKEN_INFO_HPP

#include <string_view>

// ======================
// -- ENUMS / STRUCTS
// ======================

struct CommandInfo;

enum class TokenType
{
    NUMBER,
    COMMAND,
    BRACE_OPEN,
    BRACE_CLOSE,
    ESCAPED_BRACE_OPEN,
    ESCAPED_BRACE_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    PAREN_OPEN,
    PAREN_CLOSE,
    DISPLAY_MATH_OPEN,
    DISPLAY_MATH_CLOSE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    IDENTIFIER,
    CARET,
    SUBSCRIPT,
    SUPERSCRIPT,
    DOLLAR,
    ALIGNMENT,
    PUNCTUATION,
    SPACING,
    NEWLINE,
    EQUAL,
    FACTORIAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    NOT_EQUAL,
    WHITESPACE,
    END_OF_FILE,
    INVALID,
    UNKNOWN
};

struct Token
{
    std::string_view Value;            // std::string_view: Value of the string
    const CommandInfo *Info = nullptr; // CommandInfo: Metadata for each token
    TokenType Type;                    // TokenType: Type of token
    int line;                          // int: Current line
    int column;                        // int: Current column
};

#endif
