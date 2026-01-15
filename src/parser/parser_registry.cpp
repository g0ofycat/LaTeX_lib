#include <charconv>
#include <sstream>
#include <string>

#include "./parser.hpp"
#include "../lexer/token_info.hpp"
#include "../ast/ast_node.hpp"

// ======================
// -- HELPER IMPL.
// ======================

/// @brief Check if at end of token stream
/// @return True if at end
bool Parser::is_at_end() const
{
    return _position >= _tokens.size() || _tokens[_position].Type == TokenType::END_OF_FILE;
}

/// @brief Get the current token
/// @return Current token before advancing
Token Parser::current() const
{
    if (is_at_end())
        return _tokens.back();

    return _tokens[_position];
}

/// @brief Peek at next
/// @return Current token
Token Parser::peek_next() const
{
    if (_position + 1 >= _tokens.size())
        return _tokens.back();

    return _tokens[_position + 1];
}

/// @brief Consume the current token
/// @return Token
Token Parser::consume()
{
    if (is_at_end())
        return _tokens.back();

    return _tokens[_position++];
}

/// @brief Check if current token matches given type
/// @param type: Token type to match
/// @return True if current token matches type
bool Parser::match(TokenType type)
{
    if (is_at_end())
        return false;

    return current().Type == type;
}

/// @brief Convert token type to string
/// @param type: The current type
/// @return std::string
std::string Parser::token_type_to_string(TokenType type) const
{
    switch (type)
    {
    case TokenType::NUMBER:
        return "number";
    case TokenType::IDENTIFIER:
        return "identifier";
    case TokenType::COMMAND:
        return "command";
    case TokenType::PLUS:
        return "'+'";
    case TokenType::MINUS:
        return "'-'";
    case TokenType::STAR:
        return "'*'";
    case TokenType::SLASH:
        return "'/'";
    case TokenType::EQUAL:
        return "'='";
    case TokenType::BRACE_OPEN:
        return "'{'";
    case TokenType::BRACE_CLOSE:
        return "'}'";
    case TokenType::BRACKET_OPEN:
        return "'['";
    case TokenType::BRACKET_CLOSE:
        return "']'";
    case TokenType::PUNCTUATION:
        return "punctuation";
    case TokenType::END_OF_FILE:
        return "end of input";
    default:
        return "unknown token";
    }
}

/// @brief Expect a specific token type or throw error
/// @param type: Expected token type
/// @return The matched token
Token Parser::expect(TokenType type)
{
    if (!match(type))
    {
        std::string msg = "Expected '" + token_type_to_string(type) +
                          "', but found '" + token_repr(current()) + "'";
        throw ParseError(msg, current().line, current().column);
    }

    return consume();
}

/// @brief Get string representation of a token
/// @param token: The token to represent
/// @return std::string
std::string Parser::token_repr(const Token &t) const
{
    return "'" + std::string(t.Value) + "' (" + token_type_to_string(t.Type) + ")";
}

// ======================
// -- PARSING IMPL.
// ======================

/// @brief Parse tokens into an AST
/// @return Root node of the AST
/// @throws ParseError if parsing fails
std::unique_ptr<ASTNode> Parser::parse()
{
    auto node = parse_root();

    if (!is_at_end() && current().Type != TokenType::END_OF_FILE)
    {
        std::string msg = "Unexpected token '" + token_repr(current()) +
                          "' after complete expression";
        throw ParseError(msg, current().line, current().column);
    }

    return node;
}

/// @brief Parse root of the AST
/// @return Root node of the AST
std::unique_ptr<ASTNode> Parser::parse_root()
{
    return parse_statement();
}

/// @brief Parse a statement
/// @return Statement AST node
std::unique_ptr<ASTNode> Parser::parse_statement()
{
    return parse_assignment();
}

// ======================
// -- PRECEDENCE IMPL.
// ======================

/// @brief Parse a assignment statement
/// @return AST node for assignment
std::unique_ptr<ASTNode> Parser::parse_assignment()
{
    auto left = parse_relational();

    if (match(TokenType::EQUAL))
    {
        Token eq = consume();

        auto right = parse_assignment();

        return std::make_unique<AssignNode>(
            std::move(left), std::move(right), eq.line, eq.column);
    }

    return left;
}

/// @brief Parse relational expressions (lower precedence than +/-)
/// @return AST node for relational expression
/// @note Handles: =, <, >, <=, >=, etc.
std::unique_ptr<ASTNode> Parser::parse_relational()
{
    auto left = parse_expression();

    if (match(TokenType::EQUAL))
    {
        Token op = consume();
        auto right = parse_expression();

        return std::make_unique<BinaryOpNode>(
            '=', std::move(left), std::move(right), op.line, op.column);
    }

    return left;
}

/// @brief Parse a complete expression (lowest precedence)
/// @return AST node for expression
/// @note Handles: addition, subtraction
std::unique_ptr<ASTNode> Parser::parse_expression()
{
    auto left = parse_term();

    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        Token op = consume();
        auto right = parse_term();

        char oper = (op.Type == TokenType::PLUS) ? '+' : '-';

        left = std::make_unique<BinaryOpNode>(
            oper, std::move(left), std::move(right), op.line, op.column);
    }

    return left;
}

/// @brief Parse a term (medium precedence)
/// @return AST node for term
/// @note Handles: multiplication, division
std::unique_ptr<ASTNode> Parser::parse_term()
{
    auto left = parse_power();

    while (match(TokenType::STAR) || match(TokenType::SLASH))
    {
        Token op = consume();
        auto right = parse_power();

        char oper = (op.Type == TokenType::STAR) ? '*' : '/';

        left = std::make_unique<BinaryOpNode>(
            oper, std::move(left), std::move(right), op.line, op.column);
    }

    return left;
}

/// @brief Parse a power/exponentiation (higher precedence)
/// @return AST node for power
std::unique_ptr<ASTNode> Parser::parse_power()
{
    auto base = parse_prefix();

    if (match(TokenType::CARET) || match(TokenType::SUPERSCRIPT))
    {
        Token op = consume();
        auto exponent = parse_power();

        return std::make_unique<BinaryOpNode>(
            '^', std::move(base), std::move(exponent), op.line, op.column);
    }

    return base;
}

/// @brief Parse a factor (higher precedence)
/// @return AST node for factor
std::unique_ptr<ASTNode> Parser::parse_prefix()
{
    if (match(TokenType::MINUS) || match(TokenType::PLUS))
    {
        Token op = consume();
        auto expr = parse_prefix();

        char oper = (op.Type == TokenType::MINUS) ? '-' : '+';

        return std::make_unique<UnaryOpNode>(
            oper, std::move(expr), op.line, op.column);
    }

    auto base = parse_primary();

    return parse_subsup(std::move(base));
}

/// @brief Parse a primary expression (highest precedence)
/// @return AST node for primary
/// @note Handles: numbers, variables, symbols, commands, grouping
std::unique_ptr<ASTNode> Parser::parse_primary()
{
    Token tok = current();

    switch (tok.Type)
    {
    case TokenType::NUMBER:
    {
        consume();
        double val = 0.0;
        auto [ptr, ec] = std::from_chars(tok.Value.data(),
                                         tok.Value.data() + tok.Value.size(), val);
        if (ec != std::errc{})
        {
            throw ParseError("Invalid number", tok.line, tok.column);
        }

        auto node = std::make_unique<NumberNode>(val, tok.line, tok.column);

        return try_implicit_mul(std::move(node));
    }

    case TokenType::IDENTIFIER:
    {
        consume();

        auto node = std::make_unique<VariableNode>(tok.Value, tok.line, tok.column);

        return try_implicit_mul(std::move(node));
    }

    case TokenType::COMMAND:
        return parse_command();

    case TokenType::BRACE_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::BRACE_CLOSE);

        return try_implicit_mul(std::move(expr));
    }

    case TokenType::PAREN_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::PAREN_CLOSE);

        return try_implicit_mul(std::move(expr));
    }

    default:
    {
        std::string msg = "Unexpected token in primary: " + token_repr(tok);
        throw ParseError(msg, tok.line, tok.column);
    }
    }
}

// ======================
// -- LATEX IMPL.
// ======================

/// @brief Parse a LaTeX command
/// @return AST node for command
std::unique_ptr<ASTNode> Parser::parse_command()
{
    Token cmd = consume();

    std::vector<std::unique_ptr<ASTNode>> args;

    while (match(TokenType::BRACKET_OPEN) || match(TokenType::BRACE_OPEN))
    {
        bool optional = match(TokenType::BRACKET_OPEN);

        consume();
        args.push_back(parse_expression());
        expect(optional ? TokenType::BRACKET_CLOSE : TokenType::BRACE_CLOSE);
    }

    auto node = std::make_unique<CommandNode>(
        cmd.Value, std::move(args), cmd.Info, cmd.line, cmd.column);

    return parse_subsup(std::move(node));
}

/// @brief Parse subscripts and superscripts
/// @param base: Base AST node
/// @return AST node for subscript / superscript
std::unique_ptr<ASTNode> Parser::parse_subsup(std::unique_ptr<ASTNode> base)
{
    while (match(TokenType::SUBSCRIPT) || match(TokenType::SUPERSCRIPT))
    {
        bool is_super = match(TokenType::SUPERSCRIPT);
        consume();

        std::unique_ptr<ASTNode> script;

        if (match(TokenType::BRACE_OPEN))
        {
            consume();

            script = parse_expression();
            expect(TokenType::BRACE_CLOSE);
        }
        else
        {
            script = parse_prefix();
        }

        base = std::make_unique<ScriptNode>(
            std::move(base),
            is_super ? std::move(script) : nullptr,
            is_super ? nullptr : std::move(script),
            base->line,
            base->column);
    }

    return base;
}

// ======================
// -- MISC IMPL.
// ======================

/// @brief Try to parse implicit multiplication
/// @param left: Left AST node
/// @return AST node for implicit multiplication or left node if no implicit multiplication
std::unique_ptr<ASTNode> Parser::try_implicit_mul(std::unique_ptr<ASTNode> left)
{
    if (is_at_end())
        return left;

    const Token &next = current();

    bool should_mul = false;

    if (left->Type == ASTNodeType::NUMBER ||
        left->Type == ASTNodeType::VARIABLE ||
        left->Type == ASTNodeType::COMMAND ||
        left->Type == ASTNodeType::GROUP)
    {

        should_mul = (next.Type == TokenType::NUMBER ||
                      next.Type == TokenType::IDENTIFIER ||
                      next.Type == TokenType::COMMAND ||
                      next.Type == TokenType::PAREN_OPEN);
    }

    if (!should_mul)
        return left;

    auto right = parse_prefix();

    return std::make_unique<BinaryOpNode>(
        '*', std::move(left), std::move(right),
        left->line, left->column);
}