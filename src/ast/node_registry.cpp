#include "ast_node.hpp"
#include "ast_visitor.hpp"

// ======================
// -- INIT
// ======================

/// @brief Visit a number node
/// @param node: The current node
void NumberNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a variable node
/// @param node: The current node
void VariableNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a symbol node
/// @param node: The current node
void SymbolNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a group node
/// @param node: The current node
void GroupNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a binary node
/// @param node: The current node
void BinaryOpNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a unary node
/// @param node: The current node
void UnaryOpNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a function node
/// @param node: The current node
void FunctionNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a fraction node
/// @param node: The current node
void FractionNode::accept(ASTVisitor& v) { v.visit(*this); }

/// @brief Visit a sqrt node
/// @param node: The current node
void SqrtNode::accept(ASTVisitor& v) { v.visit(*this); }
