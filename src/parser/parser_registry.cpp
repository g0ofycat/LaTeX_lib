#include <charconv>
#include <sstream>
#include <string>
#include <array>

#include "./parser.hpp"
#include "../lexer/token_info.hpp"
#include "../ast/ast_node.hpp"
#include "../ast/ast_arena.hpp"

// ======================
// -- INIT
// ======================

namespace
{
    struct ImplicitMulTable
    {
        std::array<bool, 256> data{};

        constexpr ImplicitMulTable()
        {
            data[static_cast<size_t>(TokenType::NUMBER)] = true;
            data[static_cast<size_t>(TokenType::IDENTIFIER)] = true;
            data[static_cast<size_t>(TokenType::COMMAND)] = true;
            data[static_cast<size_t>(TokenType::PAREN_OPEN)] = true;
            data[static_cast<size_t>(TokenType::BRACE_OPEN)] = true;
            data[static_cast<size_t>(TokenType::ESCAPED_BRACE_OPEN)] = true;
            data[static_cast<size_t>(TokenType::SPACING)] = true;
        }
    };

    static constexpr ImplicitMulTable MUL_LOOKUP;
}

// ======================
// -- HELPER IMPL.
// ======================

/// @brief Check if at end of token stream
/// @return True if at end
bool Parser::is_at_end() const
{
    return _tokens[_position].Type == TokenType::END_OF_FILE;
}

/// @brief Get the current token
/// @return Current token before advancing
Token Parser::current() const
{
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
    return _tokens[_position++];
}

/// @brief Check if current token matches given type
/// @param type: Token type to match
/// @return True if current token matches type
bool Parser::match(TokenType type)
{
    if (is_at_end()) [[unlikely]]
        return false;

    return current().Type == type;
}

/// @brief Expect a specific token type or throw error
/// @param type: Expected token type
/// @param msg: Optional message
/// @return The matched token
Token Parser::expect(TokenType type, const std::string &msg)
{
    if (_tokens[_position].Type != type)
    {
        std::string error_msg = msg.empty()
                                    ? "Expected token type " + std::to_string(static_cast<int>(type))
                                    : std::string(msg);

        Token err_tok = current();

        throw ParseError(error_msg + " but found " + token_repr(err_tok),
                         err_tok.line, err_tok.column);
    }

    return consume();
}

/// @brief Create a node
/// @tparam T Template
/// @tparam ...Args Template
/// @param ...args The node to make
/// @return T*
template <typename T, typename... Args>
T *Parser::make_node(Args &&...args)
{
    return _arena.alloc<T>(std::forward<Args>(args)...);
}

/// @brief Get string representation of a token
/// @param token: The token to represent
/// @return std::string
std::string Parser::token_repr(const Token &token) const
{
    return "'" + std::string(token.Value) + "' (" + std::to_string(static_cast<int>(token.Type)) + ")";
}

// ======================
// -- PARSING IMPL.
// ======================

/// @brief Parse tokens into an AST
/// @return Root node of the AST
/// @throws ParseError if parsing fails
ASTNode *Parser::parse()
{
    auto node = parse_root();

    if (!is_at_end())
    {
        Token current_token = current();

        throw ParseError(
            "Unexpected token " + token_repr(current_token) +
                " after complete expression @" +
                std::to_string(current_token.line) + ':' +
                std::to_string(current_token.column),
            current_token.line,
            current_token.column);
    }

    return node;
}

/// @brief Parse root of the AST
/// @return Root node of the AST
ASTNode *Parser::parse_root()
{
    std::vector<ASTNode *> lines;
    lines.reserve(8);

    while (!is_at_end())
    {
        lines.push_back(parse_statement());

        if (match(TokenType::NEWLINE))
        {
            consume();
        }
        else
        {
            break;
        }
    }

    return lines.size() > 1
               ? Parser::make_node<SequenceNode>(lines, lines[0]->line, lines[0]->column)
               : lines[0];
}

/// @brief Parse a statement
/// @return Statement AST node
ASTNode *Parser::parse_statement()
{
    return parse_assignment();
}

// ======================
// -- PRECEDENCE IMPL.
// ======================

/// @brief Parse a assignment statement
/// @return AST node for assignment
ASTNode *Parser::parse_assignment()
{
    ASTNode *left;

    if (match(TokenType::EQUAL) || match(TokenType::ALIGNMENT))
    {
        left = Parser::make_node<SymbolNode>("", current().line, current().column);
    }
    else
    {
        left = parse_relational();
    }

    if (match(TokenType::EQUAL) || match(TokenType::ALIGNMENT))
    {
        Token op = consume();
        auto right = parse_assignment();

        if (op.Type == TokenType::EQUAL)
        {
            return Parser::make_node<AssignNode>(left, right, op.line, op.column);
        }
        else
        {
            return Parser::make_node<BinaryOpNode>('&', left, right, op.line, op.column);
        }
    }

    return left;
}

/// @brief Parse relational expressions (lower precedence than +/-)
/// @return AST node for relational expression
/// @note Handles: =, <, >, <=, >=, etc.
ASTNode *Parser::parse_relational()
{
    auto left = parse_expression();

    TokenType current_token_type = current().Type;

    while (current_token_type == TokenType::LESS || current_token_type == TokenType::GREATER ||
           current_token_type == TokenType::LESS_EQUAL || current_token_type == TokenType::GREATER_EQUAL)
    {
        Token op = consume();
        auto right = parse_expression();

        char op_mapped = current_token_type == TokenType::LESS ? '<' : current_token_type == TokenType::GREATER  ? '>'
                                                                   : current_token_type == TokenType::LESS_EQUAL ? 'L'
                                                                                                                 : 'G';

        left = Parser::make_node<BinaryOpNode>(op_mapped, left, right,
                                               op.line, op.column);

        current_token_type = current().Type;
    }

    return left;
}

/// @brief Parse a complete expression (lowest precedence)
/// @return AST node for expression
/// @note Handles: addition, subtraction
ASTNode *Parser::parse_expression()
{
    auto left = parse_term();

    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        Token op = consume();
        auto right = parse_term();

        char oper = (op.Type == TokenType::PLUS) ? '+' : '-';

        left = Parser::make_node<BinaryOpNode>(
            oper, left, right, op.line, op.column);
    }

    return left;
}

/// @brief Parse a term (medium precedence)
/// @return AST node for term
/// @note Handles: multiplication, division
ASTNode *Parser::parse_term()
{
    auto left = parse_power();

    while (match(TokenType::STAR) || match(TokenType::SLASH))
    {
        Token op = consume();
        auto right = parse_power();

        char oper = (op.Type == TokenType::STAR) ? '*' : '/';

        left = Parser::make_node<BinaryOpNode>(
            oper, left, right, op.line, op.column);
    }

    return left;
}

/// @brief Parse a power/exponentiation (higher precedence)
/// @return AST node for power
ASTNode *Parser::parse_power()
{
    auto base = parse_prefix();

    if (match(TokenType::CARET) || match(TokenType::SUPERSCRIPT))
    {
        Token op = consume();
        auto exponent = parse_power();

        return Parser::make_node<BinaryOpNode>(
            '^', base, exponent, op.line, op.column);
    }

    return base;
}

/// @brief Parse a factor (higher precedence)
/// @return AST node for factor
ASTNode *Parser::parse_prefix()
{
    if (match(TokenType::MINUS) || match(TokenType::PLUS))
    {
        Token op = consume();
        auto expr = parse_prefix();

        char oper = (op.Type == TokenType::MINUS) ? '-' : '+';

        return Parser::make_node<UnaryOpNode>(
            oper, expr, op.line, op.column);
    }

    return parse_postfix();
}

/// @brief Parse a postfix expression (medium precedence)
/// @return AST node for postfix
ASTNode *Parser::parse_postfix()
{
    auto expr = parse_primary();

    while (true)
    {
        if (match(TokenType::PAREN_OPEN))
        {
            expr = try_function_call(expr);
        }
        else if (match(TokenType::BRACE_OPEN) || match(TokenType::ESCAPED_BRACE_OPEN))
        {
            expr = try_braced_call(expr);
        }
        else if (match(TokenType::SUBSCRIPT) || match(TokenType::SUPERSCRIPT))
        {
            expr = parse_subsup(expr);
        }
        else
        {
            break;
        }
    }

    return try_implicit_mul(expr);
}

/// @brief Parse a primary expression (highest precedence)
/// @return AST node for primary
/// @note Handles: numbers, variables, symbols, commands, grouping
ASTNode *Parser::parse_primary()
{
    Token current_token = current();

    switch (current_token.Type)
    {
    case TokenType::NUMBER:
    {
        consume();
        double val = 0.0;
        auto [ptr, ec] = std::from_chars(current_token.Value.data(),
                                         current_token.Value.data() + current_token.Value.size(), val);
        if (ec != std::errc{})
        {
            throw ParseError("Invalid number @" + std::to_string(current_token.line) + ':' + std::to_string(current_token.column), current_token.line, current_token.column);
        }

        return Parser::make_node<NumberNode>(val, current_token.line, current_token.column);
    }

    case TokenType::IDENTIFIER:
    {
        consume();

        return Parser::make_node<VariableNode>(current_token.Value, current_token.line, current_token.column);
    }

    case TokenType::COMMAND:
    {
        return parse_command();
    }

    case TokenType::ESCAPED_BRACE_OPEN:
    {
        Token tok = consume();

        auto expr = parse_expression();
        expect(TokenType::ESCAPED_BRACE_CLOSE);

        return Parser::make_node<GroupNode>(expr, tok.line, tok.column);
    }

    case TokenType::BRACE_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::BRACE_CLOSE);

        return Parser::make_node<GroupNode>(
            expr,
            current_token.line,
            current_token.column);
    }

    case TokenType::PAREN_OPEN:
    {
        consume();

        auto expr = parse_assignment();
        expect(TokenType::PAREN_CLOSE);

        return Parser::make_node<GroupNode>(
            expr,
            current_token.line,
            current_token.column);
    }

    case TokenType::DISPLAY_MATH_OPEN:
    {
        consume();

        auto expr = parse_expression();
        expect(TokenType::DISPLAY_MATH_CLOSE);

        return Parser::make_node<GroupNode>(expr, current_token.line, current_token.column);
    }

    case TokenType::SPACING:
    case TokenType::ALIGNMENT:
    case TokenType::UNKNOWN:
    {
        Token tok = consume();
        return Parser::make_node<SymbolNode>(tok.Value, tok.line, tok.column);
    }

    default:
    {
        std::string msg = "Unexpected token in primary: " + token_repr(current_token) + " @" + std::to_string(current_token.line) + ':' + std::to_string(current_token.column);
        throw ParseError(msg, current_token.line, current_token.column);
    }
    }
}

// ======================
// -- LATEX IMPL.
// ======================

/// @brief Parse a LaTeX command
/// @return AST node for command
ASTNode *Parser::parse_command()
{
    Token cmd_token = consume();
    const CommandInfo *info = cmd_token.Info;

    if (!info)
    {
        return Parser::make_node<SymbolNode>(cmd_token.Value, cmd_token.line, cmd_token.column);
    }

    std::vector<ASTNode *> args;

    args.reserve(info->mandatory_args + info->optional_args);

    for (int i = 0; i < info->optional_args; ++i)
    {
        if (match(TokenType::BRACKET_OPEN))
        {
            consume();

            args.push_back(parse_assignment());
            expect(TokenType::BRACKET_CLOSE, "Expected ']' after optional argument");
        }
        else
        {
            args.push_back(nullptr);
        }
    }

    for (int i = 0; i < info->mandatory_args; ++i)
    {
        if (match(TokenType::BRACE_OPEN))
        {
            consume();
            args.push_back(parse_assignment());
            expect(TokenType::BRACE_CLOSE, "Expected '}' after mandatory argument");
        }
        else
        {
            args.push_back(parse_primary());
        }
    }

    size_t expected = info->mandatory_args + info->optional_args;

    if (args.size() != expected)
    {
        throw ParseError(
            "Command '" + std::string(cmd_token.Value) + "' expects " +
                std::to_string(expected) + " arguments, got " + std::to_string(args.size()) + " @" + std::to_string(cmd_token.line) + ':' + std::to_string(cmd_token.column),
            cmd_token.line, cmd_token.column);
    }

    return Parser::make_node<CommandNode>(
        cmd_token.Value, args, info, cmd_token.line, cmd_token.column);
}

/// @brief Parse subscripts and superscripts
/// @param base: Base AST node
/// @return AST node for subscript / superscript
ASTNode *Parser::parse_subsup(ASTNode *base)
{
    ASTNode *sub = nullptr;
    ASTNode *sup = nullptr;

    while (match(TokenType::SUBSCRIPT) || match(TokenType::SUPERSCRIPT))
    {
        bool is_super = match(TokenType::SUPERSCRIPT);
        consume();

        if ((is_super && sup) || (!is_super && sub))
        {
            throw ParseError("Multiple scripts of the same type detected", current().line, current().column);
        }

        ASTNode *script;

        if (match(TokenType::BRACE_OPEN))
        {
            consume();

            script = parse_assignment();
            expect(TokenType::BRACE_CLOSE);
        }
        else
        {
            script = parse_prefix();
        }

        if (is_super)
            sup = script;

        else
            sub = script;
    }

    return Parser::make_node<ScriptNode>(base, sub, sup, base->line, base->column);
}

// ======================
// -- MISC IMPL.
// ======================

/// @brief Try to parse implicit multiplication
/// @param left: Left AST node
/// @return AST node for implicit multiplication or left node if no implicit multiplication
ASTNode *Parser::try_implicit_mul(ASTNode *left)
{
    while (!is_at_end())
    {
        if (!MUL_LOOKUP.data[static_cast<size_t>(current().Type)])
            break;

        if (match(TokenType::EQUAL) || match(TokenType::PLUS) || match(TokenType::MINUS))
            break;

        size_t last_pos = _position;
        auto right = parse_prefix();

        if (_position == last_pos)
        {
            break;
        }

        left = Parser::make_node<BinaryOpNode>('*', left, right,
                                               left->line, left->column);
    }

    return left;
}

/// @brief Try a function call
/// @param func The function
/// @return AST node for function call or no function call
ASTNode *Parser::try_function_call(ASTNode *func)
{
    if (!match(TokenType::PAREN_OPEN))
    {
        return func;
    }

    Token open_paren = consume();

    std::vector<ASTNode *> args;

    if (!match(TokenType::PAREN_CLOSE))
    {
        args.reserve(4);
        args.push_back(parse_assignment());

        while (match(TokenType::PUNCTUATION) && current().Value == ",")
        {
            consume();
            args.push_back(parse_assignment());
        }
    }

    expect(TokenType::PAREN_CLOSE, "Expected ')' after function arguments");

    return Parser::make_node<FunctionCallNode>(
        func,
        args,
        open_paren.line,
        open_paren.column);
}

/// @brief Try to parse arguments in curly braces
/// @param base The preceding node (the "function" or "operator")
/// @return AST node representing the applying of the braces to the base
ASTNode *Parser::try_braced_call(ASTNode *base)
{
    bool is_escaped = (current().Type == TokenType::ESCAPED_BRACE_OPEN);

    Token opening = consume();

    auto arg = parse_assignment();

    if (is_escaped)
    {
        expect(TokenType::ESCAPED_BRACE_CLOSE, "Expected '\\}' after escaped group");
    }
    else
    {
        expect(TokenType::BRACE_CLOSE, "Expected '}' after group");
    }

    std::vector<ASTNode *> args;
    args.push_back(arg);

    return Parser::make_node<FunctionCallNode>(
        base, args, opening.line, opening.column);
}