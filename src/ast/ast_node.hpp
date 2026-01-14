#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <memory>
#include <vector>
#include <string_view>

#include "./ast_info.hpp"
#include "../parser/data/latex_commands.hpp"

// ======================
// -- ASTVisitor
// ======================

class ASTVisitor;

// ======================
// -- ASTNode
// ======================

class ASTNode
{
public:
    // ======================
    // -- PUBLIC DATA
    // ======================

    ASTNodeType Type;
    SemanticInfo Info;
    int line;
    int column;

    // ======================
    // -- DESTRUCTOR
    // ======================

    virtual ~ASTNode() = default;

    virtual void accept(ASTVisitor &visitor) = 0;

protected:
    /// @brief Construct an AST node
    /// @param t: Node type
    /// @param l: Source line
    /// @param c: Source column
    ASTNode(ASTNodeType t, int l, int c)
        : Type(t), line(l), column(c) {}
};

// ======================
// -- LITERAL CLASSES
// ======================

/// @brief Numeric literal node
class NumberNode : public ASTNode
{
public:
    double value;

    NumberNode(double val, int line, int col)
        : ASTNode(ASTNodeType::NUMBER, line, col), value(val) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Variable identifier node
class VariableNode : public ASTNode
{
public:
    std::string_view name;

    VariableNode(std::string_view n, int line, int col)
        : ASTNode(ASTNodeType::VARIABLE, line, col), name(n) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Symbol node (e.g. \pi)
class SymbolNode : public ASTNode
{
public:
    std::string_view symbol;

    SymbolNode(std::string_view sym, int line, int col)
        : ASTNode(ASTNodeType::SYMBOL, line, col), symbol(sym) {}

    void accept(ASTVisitor &v) override;
};

// ======================
// -- OPERATOR CLASSES
// ======================

class GroupNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> elements;

    GroupNode(std::vector<std::unique_ptr<ASTNode>> elms, int l, int c)
        : ASTNode(ASTNodeType::GROUP, l, c), elements(std::move(elms)) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Binary operation node
class BinaryOpNode : public ASTNode
{
public:
    char op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(char operation, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r, int line, int col)
        : ASTNode(ASTNodeType::BINARY_OP, line, col),
          op(operation), left(std::move(l)), right(std::move(r)) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Unary operation node
class UnaryOpNode : public ASTNode
{
public:
    char op;
    std::unique_ptr<ASTNode> operand;

    UnaryOpNode(char operation, std::unique_ptr<ASTNode> operand, int line, int col)
        : ASTNode(ASTNodeType::UNARY_OP, line, col),
          op(operation), operand(std::move(operand)) {}

    void accept(ASTVisitor &v) override;
};

// ======================
// -- COMMAND NODE
// ======================

/// @brief Generic LaTeX Command node
class CommandNode : public ASTNode
{
public:
    std::string_view name;
    std::vector<std::unique_ptr<ASTNode>> arguments;
    const CommandInfo *cmdInfo;

    CommandNode(std::string_view n,
                std::vector<std::unique_ptr<ASTNode>> args,
                const CommandInfo *info,
                int l, int c)
        : ASTNode(ASTNodeType::COMMAND, l, c),
          name(n),
          arguments(std::move(args)),
          cmdInfo(info) {}

    void accept(ASTVisitor &v) override;
};

#endif
