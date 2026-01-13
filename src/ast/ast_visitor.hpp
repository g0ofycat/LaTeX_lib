#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

// ======================
// -- FORWARD DECLR.
// ======================

class NumberNode;
class VariableNode;
class SymbolNode;
class GroupNode;
class BinaryOpNode;
class UnaryOpNode;
class CommandNode;

// ======================
// -- ASTVisitor
// ======================

class ASTVisitor
{
public:
    // ======================
    // -- DESTRUCTOR
    // ======================

    virtual ~ASTVisitor() = default;

    // ======================
    // -- PUBLIC METHODS
    // ======================

    /// @brief Visit a number node
    /// @param node: The current node
    virtual void visit(NumberNode &node) = 0;

    /// @brief Visit a variable node
    /// @param node: The current node
    virtual void visit(VariableNode &node) = 0;

    /// @brief Visit a symbol node
    /// @param node: The current node
    virtual void visit(SymbolNode &node) = 0;

    /// @brief Visit a group node
    /// @param node: The current node
    virtual void visit(GroupNode &node) = 0;

    /// @brief Visit a binary node
    /// @param node: The current node
    virtual void visit(BinaryOpNode &node) = 0;

    /// @brief Visit a unary node
    /// @param node: The current node
    virtual void visit(UnaryOpNode &node) = 0;

    /// @brief Visit a command node
    /// @param node: The current node
    virtual void visit(CommandNode &node) = 0;
};

#endif
