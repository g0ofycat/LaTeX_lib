#include "./semantic_analyzer.hpp"

// ======================
// -- INIT
// ======================

const std::unordered_map<std::string_view, SemanticAnalyzer::ValidatorFunc> SemanticAnalyzer::VALIDATOR_DISPATCH_TABLE = {
    {"\\frac", [](SemanticAnalyzer *s, CommandNode &n)
     {
         if (n.arguments.size() >= 2)
             s->check_division_by_zero(n.arguments[1].get());
     }},
    {"\\sqrt", [](SemanticAnalyzer *s, CommandNode &n)
     {
         size_t radicand_idx = n.arguments.size() == 2 ? 1 : 0;

         if (radicand_idx < n.arguments.size() && n.arguments[radicand_idx])
         {
             s->validate_sqrt(n.arguments[radicand_idx].get(), n.line, n.column);
         }
     }},
    {"\\log", [](SemanticAnalyzer *s, CommandNode &n)
     {
         if (!n.arguments.empty())
             s->validate_log(n.arguments[0].get(), n.line, n.column);
     }},
    {"\\ln", [](SemanticAnalyzer *s, CommandNode &n)
     {
         if (!n.arguments.empty())
             s->validate_log(n.arguments[0].get(), n.line, n.column);
     }}};

// ======================
// -- FUNCTION IMPL.
// ======================

/// @brief Validate frac
/// @param node: The current node
void SemanticAnalyzer::validate_frac(CommandNode &node)
{
    if (node.arguments.size() < 2)
        return;
    check_division_by_zero(node.arguments[1].get());
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