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
    GROUP,
    BINARY_OP,
    UNARY_OP,
    COMMAND
};

struct SemanticInfo
{
    std::string InferredType;
    bool IsConstant = false;
};

#endif
