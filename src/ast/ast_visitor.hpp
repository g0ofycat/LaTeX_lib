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
class FunctionNode;
class FractionNode;
class SqrtNode;

// ======================
// -- ASTVisitor
// ======================

class ASTVisitor {
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
        virtual void visit(NumberNode& node) = 0;

        /// @brief Visit a variable node
        /// @param node: The current node
        virtual void visit(VariableNode& node) = 0;

        /// @brief Visit a symbol node
        /// @param node: The current node
        virtual void visit(SymbolNode& node) = 0;

        /// @brief Visit a group node
        /// @param node: The current node
        virtual void visit(GroupNode& node) = 0;

        /// @brief Visit a binary node
        /// @param node: The current node
        virtual void visit(BinaryOpNode& node) = 0;

        /// @brief Visit a unary node
        /// @param node: The current node
        virtual void visit(UnaryOpNode& node) = 0;

        /// @brief Visit a function node
        /// @param node: The current node
        virtual void visit(FunctionNode& node) = 0;

        /// @brief Visit a fraction node
        /// @param node: The current node
        virtual void visit(FractionNode& node) = 0;

        /// @brief Visit a sqrt node
        /// @param node: The current node
        virtual void visit(SqrtNode& node) = 0;
};

#endif
