#include "ast_node.hpp"
#include "ast_visitor.hpp"

// ======================
// -- INIT
// ======================

/// @brief Visit a number node
/// @param node: The current node
void NumberNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a variable node
/// @param node: The current node
void VariableNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a symbol node
/// @param node: The current node
void SymbolNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a group node
/// @param node: The current node
void GroupNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a binary node
/// @param node: The current node
void BinaryOpNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a unary node
/// @param node: The current node
void UnaryOpNode::accept(ASTVisitor &v) { v.visit(*this); }

/// @brief Visit a command node
/// @param node: The current node
void CommandNode::accept(ASTVisitor &v) { v.visit(*this); }
