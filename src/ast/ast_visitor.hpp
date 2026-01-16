#ifndef AST_VISITOR_HPP
#define AST_VISITOR_HPP

#include "./ast_node.hpp"

// ======================
// -- FORWARD DECLR.
// ======================

class NumberNode;
class VariableNode;
class SymbolNode;
class AssignNode;
class GroupNode;
class BinaryOpNode;
class UnaryOpNode;
class CommandNode;
class ScriptNode;
class FunctionCallNode;
class SequenceNode;

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

    /// @brief Visit a assignment node
    /// @param node: The current node
    virtual void visit(AssignNode &node) = 0;

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

    /// @brief Visit a script node
    /// @param node: The current node
    virtual void visit(ScriptNode &node) = 0;

    /// @brief Visit a function call node
    /// @param node: The current node
    virtual void visit(FunctionCallNode &node) = 0;

    /// @brief Visit a sequence node
    /// @param node: The current node
    virtual void visit(SequenceNode &node) = 0;
};

#endif
