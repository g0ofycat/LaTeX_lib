#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <utility>
#include <vector>
#include <functional>

#include "../ast/ast_node.hpp"
#include "../ast/ast_visitor.hpp"

// ======================
// -- SemanticError
// ======================

/// @brief Construct a semantic error
/// @param msg: Error message
/// @param l: Line number where error occurred
/// @param c: Column number where error occurred
struct SemanticError
{
public:
    std::string message;
    int line;
    int column;

    SemanticError(const std::string &msg, int l, int c)
        : message(msg), line(l), column(c) {}
};

// ======================
// -- SemanticAnalyzer
// ======================

class SemanticAnalyzer : public ASTVisitor
{
private:
    // ======================
    // -- PRIVATE DATA
    // ======================

    std::vector<SemanticError> errors;
    std::unordered_set<std::string_view> defined_variables;
    std::unordered_map<std::string_view, std::pair<bool, std::pair<int, int>>> variable_usage;

    // ======================
    // -- DISPATCH DATA
    // ======================

    using ValidatorFunc = std::function<void(SemanticAnalyzer *, CommandNode &)>;
    static const std::unordered_map<std::string_view, ValidatorFunc> VALIDATOR_DISPATCH_TABLE;

    // ======================
    // -- PRIVATE METHODS
    // ======================

    /// @brief Visit a number node
    /// @param node: The current node
    void visit(NumberNode &node) override;

    /// @brief Visit a variable node
    /// @param node: The current node
    void visit(VariableNode &node) override;

    /// @brief Visit a symbol node
    /// @param node: The current node
    void visit(SymbolNode &node) override;

    /// @brief Visit a assignment node
    /// @param node: The current node
    void visit(AssignNode &node) override;

    /// @brief Visit a group node
    /// @param node: The current node
    void visit(GroupNode &node) override;

    /// @brief Visit a binary node
    /// @param node: The current node
    void visit(BinaryOpNode &node) override;

    /// @brief Visit a unary node
    /// @param node: The current node
    void visit(UnaryOpNode &node) override;

    /// @brief Visit a command node
    /// @param node: The current node
    void visit(CommandNode &node) override;

    /// @brief Visit a script node
    /// @param node: The current node
    void visit(ScriptNode &node) override;

    // ======================
    // -- PRIVATE UTILITY
    // ======================

    /// @brief Check division by 0
    /// @param denominator: Denominator
    void check_division_by_zero(const ASTNode *denominator);

    /// @brief Validate frac
    /// @param node: The current node
    void validate_frac(CommandNode &node);

    /// @brief Validate sqrt
    /// @param operand: Operand to check
    /// @param line: Line number
    /// @param column: Column number
    void validate_sqrt(const ASTNode *operand, int line, int column);

    /// @brief Validate log
    /// @param operand: Operand to check
    /// @param line: Line number
    /// @param column: Column number
    void validate_log(const ASTNode *operand, int line, int column);

public:
    // ======================
    // -- CONSTRUCTRUCTOR
    // ======================

    /// @brief Semantic Analyzer Constructor
    SemanticAnalyzer() = default;

    // ======================
    // -- PUBLIC METHODS
    // ======================

    /// @brief Analyze an AST for any errors
    /// @param root: AST Root
    void analyze(const std::unique_ptr<ASTNode> &root);

    /// @brief Check to see if the AST has any errors
    bool has_errors() const { return !errors.empty(); }

    /// @brief Get all errors
    /// @return const std::vector<SemanticError>&
    const std::vector<SemanticError> &get_errors() const { return errors; }
};

#endif
