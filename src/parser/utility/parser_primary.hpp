#ifndef PARSER_PRIMARY_HPP
#define PARSER_PRIMARY_HPP

#include <unordered_map>

#include "../../lexer/token_info.hpp"
#include "../../ast/ast_node.hpp"

class Parser;

// ======================
// -- PRIMARY PARSER
// ======================

class PrimaryParser
{
private:
    // ======================
    // -- DISPATCH DATA
    // ======================

    using PrimaryHandler = ASTNode *(PrimaryParser::*)();

    static const std::unordered_map<TokenType, PrimaryHandler> PRIMARY_DISPATCH;
    static const std::unordered_map<TokenType, TokenType> GROUP_CLOSERS;

    // ======================
    // -- PARSER REF
    // ======================

    Parser &_parser;
    Token _tok;

    // ======================
    // -- PRIMARY IMPL.
    // ======================

    /// @brief Parse a number literal
    /// @return NumberNode
    ASTNode *parse_number();

    /// @brief Parse an identifier
    /// @return VariableNode
    ASTNode *parse_identifier();

    /// @brief Parse an escaped brace group
    /// @return GroupNode
    ASTNode *parse_escaped_brace();

    /// @brief Parse a grouped expression with a close token from GROUP_CLOSERS
    /// @return GroupNode
    ASTNode *parse_group();

    /// @brief Parse a symbol token (punctuation, spacing, symbol, alignment)
    /// @return SymbolNode
    ASTNode *parse_symbol();

    /// @brief Delegate to Parser::parse_command
    /// @return CommandNode
    ASTNode *parse_command();

    /// @brief Delegate to Parser::parse_environment
    /// @return EnvironmentNode
    ASTNode *parse_environment();

    /// @brief Delegate to Parser::parse_left_right
    /// @return LeftRightNode
    ASTNode *parse_left_right();

public:
    // ======================
    // -- CONSTRUCTORS
    // ======================

    /// @brief Construct a PrimaryParser bound to a Parser instance
    /// @param parser: The owning parser
    /// @param tok: The current token
    explicit PrimaryParser(Parser &parser, Token tok) : _parser(parser), _tok(tok) {}

    // ======================
    // -- PUBLIC METHODS
    // ======================

    /// @brief Parse a primary expression
    /// @return AST node for primary
    ASTNode *parse();
};

#endif
