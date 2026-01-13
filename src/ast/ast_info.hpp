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
    GROUP,
    BINARY_OP,
    UNARY_OP,
    FUNCTION,
    COMMAND,
    SYMBOL
};

struct SemanticInfo
{
    std::string InferredType;
    bool IsConstant = false;
};

#endif
