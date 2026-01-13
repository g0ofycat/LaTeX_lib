#include <cmath>

#include "./semantic_analyzer.hpp"

// ======================
// -- INIT
// ======================

/// @brief Analyze an AST for any errors
/// @param root: AST Root
void SemanticAnalyzer::analyze(const std::unique_ptr<ASTNode> &root)
{
    errors.clear();
    defined_variables.clear();
    variable_usage.clear();

    if (!root)
    {
        errors.push_back({"Empty AST", 0, 0});
        return;
    }

    root->accept(*this);
}

/// @brief Visit a number node
/// @param node: The current node
void SemanticAnalyzer::visit(NumberNode &node)
{
    if (std::isnan(node.value) || std::isinf(node.value))
    {
        errors.push_back({"Invalid number value",
                          node.line,
                          node.column});
    }
}

/// @brief Visit a variable node
/// @param node: The current node
void SemanticAnalyzer::visit(VariableNode &node)
{
    variable_usage[node.name] = {true, {node.line, node.column}};
}

/// @brief Visit a symbol node
/// @param node: The current node
void SemanticAnalyzer::visit(SymbolNode &node)
{
    // no impl, symbol always valid
}

/// @brief Visit a group node
/// @param node: The current node
void SemanticAnalyzer::visit(GroupNode &node)
{
    for (const auto &element : node.elements)
    {
        if (!element)
            continue;

        element->accept(*this);
    }
}

/// @brief Visit a binary node
/// @param node: The current node
void SemanticAnalyzer::visit(BinaryOpNode &node)
{
    if (node.left)
        node.left->accept(*this);
    if (node.right)
        node.right->accept(*this);

    if (node.op == '/')
    {
        check_division_by_zero(node.right.get());
    }
}

/// @brief Visit a unary node
/// @param node: The current node
void SemanticAnalyzer::visit(UnaryOpNode &node)
{
    if (node.operand)
    {
        node.operand->accept(*this);
    }
}

/// @brief Visit a command node
/// @param node: The current node
void SemanticAnalyzer::visit(CommandNode &node)
{
    for (auto &arg : node.arguments)
    {
        if (arg)
            arg->accept(*this);
    }

    auto it = VALIDATOR_DISPATCH_TABLE.find(node.name);

    if (it != VALIDATOR_DISPATCH_TABLE.end())
    {
        (this->*(it->second))(node);
    }
}

/// @brief No division by 0
/// @param denominator: Denominator
void SemanticAnalyzer::check_division_by_zero(const ASTNode *denominator)
{
    if (!denominator)
        return;

    if (denominator->Type == ASTNodeType::NUMBER)
    {
        const auto *num = static_cast<const NumberNode *>(denominator);

        if (num->value == 0.0)
        {
            errors.push_back({"Division by zero",
                              denominator->line,
                              denominator->column});
        }
    }
}

/// @brief Validate sqrt
/// @param operand: Operand to check
/// @param line: Line number
/// @param column: Column number
void SemanticAnalyzer::validate_sqrt(const ASTNode *operand, int line, int column)
{
    if (!operand)
        return;

    if (operand->Type == ASTNodeType::NUMBER)
    {
        const auto *num = static_cast<const NumberNode *>(operand);

        if (num->value < 0.0)
        {
            errors.push_back({"Square root of negative number (requires complex numbers)",
                              line,
                              column});
        }
    }

    if (operand->Type == ASTNodeType::UNARY_OP)
    {
        const auto *unary = static_cast<const UnaryOpNode *>(operand);

        if (unary->op == '-' && unary->operand->Type == ASTNodeType::NUMBER)
        {
            errors.push_back({"Square root of negative number (requires complex numbers)",
                              line,
                              column});
        }
    }
}

/// @brief Validate log
/// @param operand: Operand to check
/// @param line: Line number
/// @param column: Column number
void SemanticAnalyzer::validate_log(const ASTNode *operand, int line, int column)
{
    if (!operand)
        return;

    if (operand->Type == ASTNodeType::NUMBER)
    {
        const auto *num = static_cast<const NumberNode *>(operand);

        if (num->value <= 0.0)
        {
            errors.push_back({"Logarithm of non-positive number is undefined",
                              line,
                              column});
        }
    }

    if (operand->Type == ASTNodeType::UNARY_OP)
    {
        const auto *unary = static_cast<const UnaryOpNode *>(operand);

        if (unary->op == '-')
        {
            errors.push_back({"Logarithm of negative number is undefined",
                              line,
                              column});
        }
    }
}
