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

/// @brief Visit a assignment node
/// @param node: The current node
void SemanticAnalyzer::visit(AssignNode &node)
{
    if (node.target)
        node.target->accept(*this);

    if (node.value)
        node.value->accept(*this);

    if (node.target && node.target->Type == ASTNodeType::VARIABLE)
    {
        const auto *var = static_cast<const VariableNode *>(node.target.get());
        defined_variables.insert(var->name);
    }

    if (node.target && node.target->Type == ASTNodeType::NUMBER)
    {
        errors.push_back({"Cannot assign to a literal value",
                          node.line,
                          node.column});
    }
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
        it->second(this, node);
    }
}

/// @brief Visit a script node
/// @param node: The current node
void SemanticAnalyzer::visit(ScriptNode &node)
{
    node.base->accept(*this);

    if (node.subscript)
    {
        node.subscript->accept(*this);
    }

    if (node.superscript)
    {
        node.superscript->accept(*this);
    }
}

/// @brief Visit a function call node
/// @param node: The current node
void SemanticAnalyzer::visit(FunctionCallNode &node)
{
    node.function->accept(*this);

    for (auto &arg : node.args)
    {
        if (!arg)
            continue;

        arg->accept(*this);
    }
}

/// @brief Check division by 0
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
