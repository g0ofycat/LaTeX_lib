#include "./parser.hpp"

// ======================
// -- METHODS
// ======================

 /// @brief Peek to the next token
/// @return Token
Token Parser::peek() const {
    if (position >= tokens.size()) {
        return tokens.back();
    }

    return tokens[position];
}

/// @brief Advance to the next token
/// @return Token
Token Parser::advance() {
    if (position >= tokens.size()) {
        return tokens.back();
    }

    return tokens[position++];
}

/// @brief Matches a type to a type
/// @param type: The current type
/// @return bool
bool Parser::match(TokenType type) {
    if (at_end()) return false;

    return peek().Type == type;
}

/// @brief Throw an error if the current token type is not the same as the next token
/// @param type: The current type
/// @return Token
Token Parser::expect(TokenType type) {
    Token current = peek();

    if (current.Type != type) {
        throw ParseError(
            "Expected token type, got different type",
            current.line,
            current.column
        );
    }

    return advance();
}

/// @brief Skip Whitespace
void Parser::skip_whitespace() {
    while (!at_end() && peek().Type == TokenType::WHITESPACE) {
        advance();
    }
}

/// @brief Expect a specific token type after skipping whitespace
/// @param type: The expected token type
/// @return The matched token
Token Parser::expect_ws(TokenType type) {
    skip_whitespace();
    return expect(type);
}

/// @brief Check to see if it's the end of the file
/// @return bool
bool Parser::at_end() const {
    return position >= tokens.size() || peek().Type == TokenType::END_OF_FILE;
}

// ======================
// -- PARSING METHODS
// ======================

/// @brief Parse a complete expression (lowest precedence)
/// @return AST node for expression
/// @note Handles: addition, subtraction
std::unique_ptr<ASTNode> Parser::parse_expression() {
    auto left = parse_term();
    
    while (true) {
        skip_whitespace();

        if (!(match(TokenType::PLUS) || match(TokenType::MINUS))) break;

        Token op = advance();
        auto right = parse_term();

        left = std::make_unique<BinaryOpNode>(
            op.Value[0],
            std::move(left),
            std::move(right),
            op.line,
            op.column
        );
    }

    return left;
}

/// @brief Parse a primary expression (highest precedence)
/// @return AST node for primary
/// @note Handles: numbers, variables, symbols, commands, grouping
std::unique_ptr<ASTNode> Parser::parse_primary() {
    skip_whitespace();

    Token current = peek();

    if (match(TokenType::NUMBER)) {
        Token num = advance();

        double value = std::stod(std::string(num.Value));

        return std::make_unique<NumberNode>(value, num.line, num.column);
    }

    if (match(TokenType::IDENTIFIER)) {
        Token var = advance();

        return std::make_unique<VariableNode>(
            std::string(var.Value), 
            var.line, 
            var.column
        );
    }

    if (match(TokenType::COMMAND)) {
        return parse_command();
    }
 
    if (match(TokenType::BRACE_OPEN)) {
        advance();

        skip_whitespace();

        auto expr = parse_expression();

        expect_ws(TokenType::BRACE_CLOSE);

        return expr;
    }

    throw ParseError(
        "Unexpected token in primary expression",
        current.line,
        current.column
    );
}

/// @brief Parse a LaTeX command
/// @return AST node for command
/// @note Handles: \sin, \frac, \sqrt, symbols like \pi
std::unique_ptr<ASTNode> Parser::parse_command() {
    Token cmd = advance();

    skip_whitespace();

    if (!cmd.Info) {
        return std::make_unique<SymbolNode>(
            std::string(cmd.Value),
            cmd.line,
            cmd.column
        );
    }

    switch (cmd.Info->Type) {
        case CommandType::SYMBOL:
            return std::make_unique<SymbolNode>(
                std::string(cmd.Value),
                cmd.line,
                cmd.column
            );
        case CommandType::UNARY:
            if (cmd.Value == "\\sqrt") {
                expect_ws(TokenType::BRACE_OPEN);

                auto operand = parse_expression();
                expect_ws(TokenType::BRACE_CLOSE);

                return std::make_unique<SqrtNode>(
                    std::move(operand),
                    cmd.line,
                    cmd.column
                );
            } else {
                expect_ws(TokenType::BRACE_OPEN);

                auto arg = parse_expression();
                expect_ws(TokenType::BRACE_CLOSE);

                return std::make_unique<FunctionNode>(
                    std::string(cmd.Value),
                    std::move(arg),
                    cmd.line,
                    cmd.column
                );
            }
        case CommandType::BINARY:
            if (cmd.Value == "\\frac") {
                expect_ws(TokenType::BRACE_OPEN);

                auto numerator = parse_expression();
                expect_ws(TokenType::BRACE_CLOSE);

                expect_ws(TokenType::BRACE_OPEN);

                auto denominator = parse_expression();
                expect_ws(TokenType::BRACE_CLOSE);

                return std::make_unique<FractionNode>(
                    std::move(numerator),
                    std::move(denominator),
                    cmd.line,
                    cmd.column
                );
            }

            break;
    }

    throw ParseError(
        "Unsupported command type",
        cmd.line,
        cmd.column
    );
}

/// @brief Parse a factor (higher precedence)
/// @return AST node for factor
/// @note Handles: implicit multiplication (2x, xy)
std::unique_ptr<ASTNode> Parser::parse_factor() {
    auto left = parse_primary();

    while (true) {
        skip_whitespace();

        if (at_end()) break;

        if (!(match(TokenType::NUMBER) ||
              match(TokenType::IDENTIFIER) ||
              match(TokenType::COMMAND) ||
              match(TokenType::BRACE_OPEN))) {
            break;
        }

        if (match(TokenType::BRACE_OPEN) &&
            left->Type == ASTNodeType::FUNCTION) {
            break;
        }

        auto right = parse_primary();

        left = std::make_unique<BinaryOpNode>(
            '*',
            std::move(left),
            std::move(right),
            left->line,
            left->column
        );
    }

    return left;
}

/// @brief Parse a term (medium precedence)
/// @return AST node for term
/// @note Handles: multiplication, division
std::unique_ptr<ASTNode> Parser::parse_term() {
    auto left = parse_factor();

    while (true) {
        skip_whitespace();

        if (!(match(TokenType::STAR) || match(TokenType::SLASH))) break;

        Token op = advance();
        auto right = parse_factor();

        left = std::make_unique<BinaryOpNode>(
            op.Value[0],
            std::move(left),
            std::move(right),
            op.line,
            op.column
        );
    }

    return left;
}

/// @brief Parse tokens into an AST
/// @return Root node of the AST
/// @throws ParseError if parsing fails
std::unique_ptr<ASTNode> Parser::parse() {
    if (at_end()) {
        throw ParseError("Empty input", 0, 0);
    }

    auto ast = parse_expression();

    if (!at_end() && peek().Type != TokenType::END_OF_FILE) {
        Token unexpected = peek();

        throw ParseError(
            "Unexpected token after expression",
            unexpected.line,
            unexpected.column
        );
    }

    return ast;
}
