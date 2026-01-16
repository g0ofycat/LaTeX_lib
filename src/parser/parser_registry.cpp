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
    case TokenType::ESCAPED_BRACE_OPEN:
        return "'\\{'";
    case TokenType::ESCAPED_BRACE_CLOSE:
        return "'\\}'";
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
/// @param msg: Optional message
/// @return The matched token
Token Parser::expect(TokenType type, const std::string &msg)
{
    if (!match(type))
    {
        std::string what = msg.empty()
                               ? "Expected '" + token_type_to_string(type) + "'"
                               : msg;

        std::string found = token_repr(current());

        throw ParseError(what + ", but found " + found,
                         current().line, current().column);
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
        std::string msg = "Unexpected token " + token_repr(current()) +
                          " after complete expression";
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

    while (match(TokenType::LESS) || match(TokenType::GREATER) || match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL))
    {
        Token op = consume();
        auto right = parse_expression();

        char op_mapped;

        switch (op.Type)
        {
        case TokenType::LESS:
            op_mapped = '<';
            break;
        case TokenType::GREATER:
            op_mapped = '>';
            break;
        case TokenType::LESS_EQUAL:
            op_mapped = 'L';
            break;
        case TokenType::GREATER_EQUAL:
            op_mapped = 'G';
            break;
        default:
            op_mapped = '?';
            break;
        }

        left = std::make_unique<BinaryOpNode>(
            op_mapped, std::move(left), std::move(right), op.line, op.column);
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

    return parse_postfix();
}

/// @brief Parse a postfix expression (medium precedence)
/// @return AST node for postfix
std::unique_ptr<ASTNode> Parser::parse_postfix()
{
    auto expr = parse_primary();

    while (true)
    {
        if (match(TokenType::PAREN_OPEN))
        {
            expr = try_function_call(std::move(expr));
        }
        else if (match(TokenType::BRACE_OPEN) || match(TokenType::ESCAPED_BRACE_OPEN))
        {
            expr = try_braced_call(std::move(expr));
        }
        else if (match(TokenType::SUBSCRIPT) || match(TokenType::SUPERSCRIPT))
        {
            expr = parse_subsup(std::move(expr));
        }
        else
        {
            break;
        }
    }

    return try_implicit_mul(std::move(expr));
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

        return std::make_unique<NumberNode>(val, tok.line, tok.column);
    }

    case TokenType::IDENTIFIER:
    {
        consume();

        return std::make_unique<VariableNode>(tok.Value, tok.line, tok.column);
    }

    case TokenType::COMMAND:
    {
        return parse_command();
    }

    case TokenType::ESCAPED_BRACE_OPEN:
    {
        Token tok = consume();

        auto expr = parse_expression();
        expect(TokenType::ESCAPED_BRACE_CLOSE, "Missing closing '\\}'");

        std::vector<std::unique_ptr<ASTNode>> elements;
        elements.push_back(std::move(expr));

        return std::make_unique<GroupNode>(std::move(elements), tok.line, tok.column);
    }

    case TokenType::BRACE_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::BRACE_CLOSE);

        std::vector<std::unique_ptr<ASTNode>> elements;
        elements.reserve(1);
        elements.emplace_back(std::move(expr));

        return std::make_unique<GroupNode>(
            std::move(elements),
            tok.line,
            tok.column);
    }

    case TokenType::PAREN_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::PAREN_CLOSE);

        std::vector<std::unique_ptr<ASTNode>> elements;
        elements.reserve(1);
        elements.emplace_back(std::move(expr));

        return std::make_unique<GroupNode>(
            std::move(elements),
            tok.line,
            tok.column);
    }

    case TokenType::UNKNOWN:
    {
        Token tok = consume();
        return std::make_unique<SymbolNode>(tok.Value, tok.line, tok.column);
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
    Token cmd_token = consume();

    const CommandInfo *info = cmd_token.Info;

    if (!info)
    {
        return std::make_unique<SymbolNode>(cmd_token.Value, cmd_token.line, cmd_token.column);
    }

    std::vector<std::unique_ptr<ASTNode>> args;

    args.reserve(info->mandatory_args + info->optional_args);

    for (int i = 0; i < info->optional_args; ++i)
    {
        if (match(TokenType::BRACKET_OPEN))
        {
            consume();

            args.push_back(parse_expression());
            expect(TokenType::BRACKET_CLOSE, "Expected ']' after optional argument");
        }
        else
        {
            args.push_back(nullptr);
        }
    }

    for (int i = 0; i < info->mandatory_args; ++i)
    {
        expect(TokenType::BRACE_OPEN, "Expected '{' for mandatory argument");
        args.push_back(parse_expression());
        expect(TokenType::BRACE_CLOSE, "Expected '}' after mandatory argument");
    }

    size_t expected = info->mandatory_args + info->optional_args;

    if (args.size() != expected)
    {
        throw ParseError(
            "Command '" + std::string(cmd_token.Value) + "' expects " +
                std::to_string(expected) + " arguments, got " + std::to_string(args.size()),
            cmd_token.line, cmd_token.column);
    }

    return std::make_unique<CommandNode>(
        cmd_token.Value, std::move(args), info, cmd_token.line, cmd_token.column);
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
    while (true)
    {
        if (is_at_end())
            break;

        const Token &next = current();

        if (next.Type == TokenType::EQUAL ||
            next.Type == TokenType::PLUS ||
            next.Type == TokenType::MINUS ||
            next.Type == TokenType::LESS ||
            next.Type == TokenType::GREATER)
        {
            break;
        }

        bool can_mul = (next.Type == TokenType::NUMBER ||
                        next.Type == TokenType::IDENTIFIER ||
                        next.Type == TokenType::COMMAND ||
                        next.Type == TokenType::PAREN_OPEN ||
                        next.Type == TokenType::BRACE_OPEN ||
                        next.Type == TokenType::ESCAPED_BRACE_OPEN);

        if (!can_mul)
            break;

        auto right = parse_prefix();
        left = std::make_unique<BinaryOpNode>('*', std::move(left), std::move(right), left->line, left->column);
    }

    return left;
}

/// @brief Try a function call
/// @param func The function
/// @return AST node for function call or no function call
std::unique_ptr<ASTNode> Parser::try_function_call(std::unique_ptr<ASTNode> func)
{
    if (!match(TokenType::PAREN_OPEN))
    {
        return func;
    }

    Token open_paren = consume();

    std::vector<std::unique_ptr<ASTNode>> args;

    if (!match(TokenType::PAREN_CLOSE))
    {
        args.push_back(parse_expression());

        while (match(TokenType::PUNCTUATION) && current().Value == ",")
        {
            consume();
            args.push_back(parse_expression());
        }
    }

    expect(TokenType::PAREN_CLOSE, "Expected ')' after function arguments");

    return std::make_unique<FunctionCallNode>(
        std::move(func),
        std::move(args),
        open_paren.line,
        open_paren.column);
}

/// @brief Try to parse arguments in curly braces
/// @param base The preceding node (the "function" or "operator")
/// @return AST node representing the applying of the braces to the base
std::unique_ptr<ASTNode> Parser::try_braced_call(std::unique_ptr<ASTNode> base)
{
    bool is_escaped = (current().Type == TokenType::ESCAPED_BRACE_OPEN);

    Token opening = consume();

    auto arg = parse_expression();

    if (is_escaped)
    {
        expect(TokenType::ESCAPED_BRACE_CLOSE, "Expected '\\}' after escaped group");
    }
    else
    {
        expect(TokenType::BRACE_CLOSE, "Expected '}' after group");
    }

    std::vector<std::unique_ptr<ASTNode>> args;
    args.push_back(std::move(arg));

    return std::make_unique<FunctionCallNode>(
        std::move(base), std::move(args), opening.line, opening.column);
}