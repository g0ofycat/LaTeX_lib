#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <memory>
#include <vector>
#include <string_view>
#include <stdexcept>

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

/// @brief Symbol node
class SymbolNode : public ASTNode
{
public:
    std::string_view symbol;

    SymbolNode(std::string_view sym, int line, int col)
        : ASTNode(ASTNodeType::SYMBOL, line, col), symbol(sym) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Assignment node
class AssignNode : public ASTNode
{
public:
    ASTNode *target;
    ASTNode *value;

    AssignNode(ASTNode *t, ASTNode *v, int line, int col)
        : ASTNode(ASTNodeType::ASSIGN, line, col),
          target(t), value(v) {}

    void accept(ASTVisitor &v) override;
};

// ======================
// -- OPERATOR CLASSES
// ======================

/// @brief Grouping node
class GroupNode : public ASTNode
{
public:
    std::vector<ASTNode *> elements;

    GroupNode(ASTNode *single, int l, int c)
        : ASTNode(ASTNodeType::GROUP, l, c)
    {
        elements.push_back(single);
    }

    GroupNode(std::vector<ASTNode *> elms, int l, int c)
        : ASTNode(ASTNodeType::GROUP, l, c), elements(elms) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Binary operation node
class BinaryOpNode : public ASTNode
{
public:
    char op;
    ASTNode *left;
    ASTNode *right;

    BinaryOpNode(char operation, ASTNode *l, ASTNode *r, int line, int col)
        : ASTNode(ASTNodeType::BINARY_OP, line, col),
          op(operation), left(l), right(r) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Unary operation node
class UnaryOpNode : public ASTNode
{
public:
    char op;
    ASTNode *operand;

    UnaryOpNode(char operation, ASTNode *operand, int line, int col)
        : ASTNode(ASTNodeType::UNARY_OP, line, col),
          op(operation), operand(operand) {}

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
    std::vector<ASTNode *> arguments;
    const CommandInfo *cmdInfo;

    CommandNode(std::string_view n,
                std::vector<ASTNode *> args,
                const CommandInfo *info,
                int l, int c)
        : ASTNode(ASTNodeType::COMMAND, l, c),
          name(n),
          arguments(args),
          cmdInfo(info) {}

    void accept(ASTVisitor &v) override;
};

// ======================
// -- OTHER NODES
// ======================

/// @brief Node representing a base with optional subscript and / or superscript
class ScriptNode : public ASTNode
{
public:
    ASTNode *base;
    ASTNode *subscript;
    ASTNode *superscript;

    ScriptNode(ASTNode *b,
               ASTNode *sub,
               ASTNode *sup,
               int line, int col)
        : ASTNode(ASTNodeType::SCRIPT, line, col),
          base(b),
          subscript(sub),
          superscript(sup)
    {
        if (!base)
        {
            throw std::invalid_argument("ScriptNode must have a non-null base");
        }
    }

    void accept(ASTVisitor &v) override;
};

/// @brief Node representing function calls
class FunctionCallNode : public ASTNode
{
public:
    ASTNode *function;
    std::vector<ASTNode *> args;

    FunctionCallNode(
        ASTNode *func,
        std::vector<ASTNode *> arguments,
        int l, int c)
        : ASTNode(ASTNodeType::FUNCTION_CALL, l, c),
          function(std::move(func)),
          args(arguments) {}

    void accept(ASTVisitor &v) override;
};

/// @brief Node representing new lines
class SequenceNode : public ASTNode
{
public:
    std::vector<ASTNode *> elements;

    SequenceNode(std::vector<ASTNode *> elems, int line, int col)
        : ASTNode(ASTNodeType::SEQUENCE, line, col),
          elements(std::move(elems)) {}

    void accept(ASTVisitor &visitor) override;
};

/// @brief Node representing an environment
class EnvironmentNode : public ASTNode
{
public:
    std::string_view name;
    std::vector<std::vector<ASTNode *>> content;

    EnvironmentNode(std::string_view n,
                    std::vector<std::vector<ASTNode *>> cont,
                    int l, int c)
        : ASTNode(ASTNodeType::ENVIRONMENT, l, c),
          name(n),
          content(std::move(cont)) {}

    void accept(ASTVisitor &visitor) override;
};

/// @brief Node representing \left <delim> ... \right <delim>
class LeftRightNode : public ASTNode
{
public:
    std::string left_delimiter;
    std::string right_delimiter;
    ASTNode *content;

    LeftRightNode(std::string left, std::string right, ASTNode *inner, int l, int c)
        : ASTNode(ASTNodeType::LEFT_RIGHT, l, c),
          left_delimiter(std::move(left)),
          right_delimiter(std::move(right)),
          content(inner) {}

    void accept(ASTVisitor &visitor) override;
};

#endif
