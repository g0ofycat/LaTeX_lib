#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <stdexcept>

#include "../lexer/token_info.hpp"
#include "../ast/ast_node.hpp"

// ======================
// -- EXCEPTIONS
// ======================

/// @brief Exception thrown during parsing errors
class ParseError : public std::runtime_error {
    public:
        int line;
        int column;

        /// @brief Construct a parse error
        /// @param msg: Error message
        /// @param l: Line number where error occurred
        /// @param c: Column number where error occurred
        ParseError(const std::string& msg, int l, int c)
            : std::runtime_error(msg), line(l), column(c) {}
};

// ======================
// -- PARSER CLASS
// ======================

class Parser {
    private:
        // ======================
        // -- PARSER DATA
        // ======================

        std::vector<Token> tokens;
        size_t position = 0;

    private:
        // ======================
        // -- HELPER METHODS
        // ======================

        /// @brief Peek at current token without advancing
        /// @return Current token
        Token peek() const;

        /// @brief Advance position and return current token
        /// @return Current token before advancing
        Token advance();

        /// @brief Check if current token matches given type
        /// @param type: Token type to match
        /// @return True if current token matches type
        bool match(TokenType type);

        /// @brief Expect a specific token type or throw error
        /// @param type: Expected token type
        /// @return The matched token
        Token expect(TokenType type);

        /// @brief Skip Whitespace
        void skip_whitespace();

        /// @brief Expect a specific token type after skipping whitespace
        /// @param type: The expected token type
        /// @return The matched token
        Token expect_ws(TokenType type);

        /// @brief Check if at end of token stream
        /// @return True if at end
        bool at_end() const;

        // ======================
        // -- PARSING METHODS
        // ======================

        /// @brief Parse a complete expression (lowest precedence)
        /// @return AST node for expression
        /// @note Handles: addition, subtraction
        std::unique_ptr<ASTNode> parse_expression();

        /// @brief Parse a primary expression (highest precedence)
        /// @return AST node for primary
        /// @note Handles: numbers, variables, symbols, commands, grouping
        std::unique_ptr<ASTNode> parse_primary();

        /// @brief Parse a LaTeX command
        /// @return AST node for command
        /// @note Handles: \sin, \frac, \sqrt, symbols like \pi
        std::unique_ptr<ASTNode> parse_command();

        /// @brief Parse a factor (higher precedence)
        /// @return AST node for factor
        /// @note Handles: implicit multiplication (2x, xy)
        std::unique_ptr<ASTNode> parse_factor();

        /// @brief Parse a term (medium precedence)
        /// @return AST node for term
        /// @note Handles: multiplication, division
        std::unique_ptr<ASTNode> parse_term();

    public:
        // ======================
        // -- CONSTRUCTOR
        // ======================

        /// @brief Parser constructor
        /// @param toks: Vector of tokens to parse
        Parser(std::vector<Token> toks) : tokens(std::move(toks)) {}

        // ======================
        // -- PUBLIC METHODS
        // ======================

        /// @brief Parse tokens into an AST
        /// @return Root node of the AST
        /// @throws ParseError if parsing fails
        std::unique_ptr<ASTNode> parse();
};

#endif
