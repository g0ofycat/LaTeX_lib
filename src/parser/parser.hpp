#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include "../lexer/token_info.hpp"
#include "../ast/ast_node.hpp"
#include "../ast/ast_arena.hpp"

// ======================
// -- EXCEPTIONS
// ======================

/// @brief Exception thrown during parsing errors
class ParseError : public std::runtime_error
{
public:
    int line;
    int column;

    /// @brief Construct a parse error
    /// @param msg: Error message
    /// @param l: Line number where error occurred
    /// @param c: Column number where error occurred
    ParseError(const std::string &msg, int l, int c)
        : std::runtime_error(msg), line(l), column(c) {}
};

// ======================
// -- PARSER CLASS
// ======================

class Parser
{
private:
    // ======================
    // -- PARSER DATA
    // ======================

    ASTArena _arena;
    std::vector<Token> _tokens;
    size_t _position = 0;

private:
    // ======================
    // -- HELPER METHODS
    // ======================

    /// @brief Check if at end of token stream
    /// @return True if at end
    [[nodiscard]] bool is_at_end() const;

    /// @brief Get the current token
    /// @return Current token before advancing
    [[nodiscard]] Token current() const;

    /// @brief Peek at next
    /// @return Current token
    [[nodiscard]] Token peek_next() const;

    /// @brief Consume the current token
    /// @return Token
    Token consume();

    /// @brief Check if current token matches given type
    /// @param type: Token type to match
    /// @return True if current token matches type
    bool match(TokenType type);

    /// @brief Expect a specific token type or throw error
    /// @param type: Expected token type
    /// @param msg: Optional message
    /// @return The matched token
    Token expect(TokenType type, const std::string &msg = "");

    /// @brief Create a node
    /// @tparam T Template
    /// @tparam ...Args Template
    /// @param ...args The node to make
    /// @return T*
    template <typename T, typename... Args>
    T *make_node(Args &&...args);

    // ======================
    // -- PARSING METHODS
    // ======================

    /// @brief Parse root of the AST
    /// @return Root node of the AST
    ASTNode *parse_root();

    /// @brief Parse a statement
    /// @return Statement AST node
    ASTNode *parse_statement();

    /// @brief Parse a assignment statement
    /// @return AST node for assignment
    ASTNode *parse_assignment();

    /// @brief Parse relational expressions (lower precedence than +/-)
    /// @return AST node for relational expression
    /// @note Handles: =, <, >, <=, >=, etc.
    ASTNode *parse_relational();

    /// @brief Parse a complete expression (lowest precedence)
    /// @return AST node for expression
    /// @note Handles: addition, subtraction
    ASTNode *parse_expression();

    /// @brief Parse a term (medium precedence)
    /// @return AST node for term
    /// @note Handles: multiplication, division
    ASTNode *parse_term();

    /// @brief Parse a power/exponentiation (higher precedence)
    /// @return AST node for power
    ASTNode *parse_power();

    /// @brief Parse a factor (higher precedence)
    /// @return AST node for factor
    ASTNode *parse_prefix();

    /// @brief Parse a postfix expression (medium precedence)
    /// @return AST node for postfix
    ASTNode *parse_postfix();

    /// @brief Parse a primary expression (highest precedence)
    /// @return AST node for primary
    /// @note Handles: numbers, variables, symbols, commands, grouping
    ASTNode *parse_primary();

    // ======================
    // -- LATEX COMMAND PARSING METHODS
    // ======================

    /// @brief Parse a LaTeX command
    /// @return AST node for command
    ASTNode *parse_command();

    /// @brief Parse subscripts and superscripts
    /// @param base: Base AST node
    /// @return AST node for subscript / superscript
    ASTNode *parse_subsup(ASTNode *base);

    /// @brief Try to parse implicit multiplication
    /// @param left: Left AST node
    /// @return AST node for implicit multiplication or left node if no implicit multiplication
    ASTNode *try_implicit_mul(ASTNode *left);

    /// @brief Try a function call
    /// @param func The function
    /// @return AST node for function call or no function call
    ASTNode *try_function_call(ASTNode *func);

    /// @brief Try to parse arguments in curly braces
    /// @param base The preceding node (the "function" or "operator")
    /// @return AST node representing the applying of the braces to the base
    ASTNode *try_braced_call(ASTNode *base);

    // ======================
    // -- UTILITY
    // ======================

    /// @brief Get string representation of a token
    /// @param token: The token to represent
    /// @return std::string
    std::string token_repr(const Token &token) const;

public:
    // ======================
    // -- CONSTRUCTORS
    // ======================

    /// @brief Default Constructor
    Parser() = default;

    /// @brief Parser constructor
    /// @param toks: Vector of tokens to parse
    Parser(std::vector<Token> toks) : _tokens(std::move(toks)) {}

    // ======================
    // -- PUBLIC METHODS
    // ======================

    /// @brief Parse tokens into an AST
    /// @return Root node of the AST
    /// @throws ParseError if parsing fails
    ASTNode *parse();
};

#endif
