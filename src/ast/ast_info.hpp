#ifndef AST_INFO_HPP
#define AST_INFO_HPP

#include <string>

// ======================
// -- ENUMS / STRUCTS
// ======================

enum class ASTNodeType
{
    NUMBER,
    VARIABLE,
    SYMBOL,
    ASSIGN,
    GROUP,
    BINARY_OP,
    UNARY_OP,
    COMMAND
};

#endif
