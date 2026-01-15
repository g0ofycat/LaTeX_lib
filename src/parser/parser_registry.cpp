#include <vector>
#include <memory>
#include <stdexcept>
#include <charconv>

#include "./parser.hpp"

// ======================
// -- METHODS
// ======================

/// @brief Peek to the next token
/// @return Token
Token Parser::peek() const
{
    if (position >= tokens.size())
    {
        return tokens.back();
    }

    return tokens[position];
}

/// @brief Advance to the next token
/// @return Token
Token Parser::advance()
{
    if (position >= tokens.size())
    {
        return tokens.back();
    }

    Token token = tokens[position++];

    while (position < tokens.size() && tokens[position].Type == TokenType::WHITESPACE)
    {
        position++;
    }

    return token;
}

/// @brief Matches a type to a type
/// @param type: The current type
/// @return bool
bool Parser::match(TokenType type)
{
    if (at_end())
        return false;

    return peek().Type == type;
}

/// @brief Throw an error if the current token type is not the same as the next token
/// @param type: The current type
/// @return Token
Token Parser::expect(TokenType type)
{
    Token current = peek();

    if (current.Type != type)
    {
        throw ParseError(
            "Expected token type, got different type",
            current.line,
            current.column);
    }

    return advance();
}

/// @brief Check to see if it's the end of the file
/// @return bool
bool Parser::at_end() const
{
    return position >= tokens.size() || peek().Type == TokenType::END_OF_FILE;
}

// ======================
// -- PARSING METHODS
// ======================

/// @brief Parse a complete expression (lowest precedence)
/// @return AST node for expression
/// @note Handles: addition, subtraction
std::unique_ptr<ASTNode> Parser::parse_expression()
{
    auto left = parse_term();

    while (!at_end())
    {
        Token current = peek();
        if (current.Type != TokenType::PLUS && current.Type != TokenType::MINUS)
            break;

        advance();
        auto right = parse_term();

        left = std::make_unique<BinaryOpNode>(
            current.Value[0],
            std::move(left),
            std::move(right),
            current.line,
            current.column);
    }

    return left;
}

/// @brief Parse a primary expression (highest precedence)
/// @return AST node for primary
/// @note Handles: numbers, variables, symbols, commands, grouping
std::unique_ptr<ASTNode> Parser::parse_primary()
{
    Token current = peek();

    switch (current.Type)
    {
    case TokenType::MINUS:
    case TokenType::PLUS:
    {
        Token op = advance();
        auto operand = parse_primary();
        return std::make_unique<UnaryOpNode>(op.Value[0], std::move(operand), op.line, op.column);
    }

    case TokenType::NUMBER:
    {
        Token num = advance();
        double value;
        auto result = std::from_chars(
            num.Value.data(),
            num.Value.data() + num.Value.size(),
            value);

        if (result.ec != std::errc())
        {
            throw ParseError("Invalid number", num.line, num.column);
        }

        return std::make_unique<NumberNode>(value, num.line, num.column);
    }

    case TokenType::IDENTIFIER:
    {
        Token var = advance();
        return std::make_unique<VariableNode>(var.Value, var.line, var.column);
    }

    case TokenType::COMMAND:
        return parse_command();

    case TokenType::BRACE_OPEN:
    {
        advance();
        auto expr = parse_expression();
        expect(TokenType::BRACE_CLOSE);
        return expr;
    }

    case TokenType::PUNCTUATION:
        if (current.Value == "(")
        {
            advance();
            auto expr = parse_expression();

            current = peek();
            if (current.Type != TokenType::PUNCTUATION || current.Value != ")")
            {
                throw ParseError("Expected closing parenthesis", current.line, current.column);
            }
            advance();
            return expr;
        }
        break;

    default:
        break;
    }

    throw ParseError("Unexpected token in primary expression", current.line, current.column);
}

/// @brief Parse a LaTeX command
/// @return AST node for command
std::unique_ptr<ASTNode> Parser::parse_command()
{
    Token cmd = advance();

    if (!cmd.Info || cmd.Info->Type == CommandType::SYMBOL)
    {
        return std::make_unique<SymbolNode>(cmd.Value, cmd.line, cmd.column);
    }

    std::vector<std::unique_ptr<ASTNode>> collected_args;

    for (ArgType arg_type : cmd.Info->arg_pattern)
    {
        if (arg_type == ArgType::OPTIONAL)
        {
            if (match(TokenType::BRACKET_OPEN))
            {
                advance();
                collected_args.push_back(parse_expression());
                expect(TokenType::BRACKET_CLOSE);
            }
            else
            {
                collected_args.push_back(nullptr);
            }
        }
        else
        {
            expect(TokenType::BRACE_OPEN);
            collected_args.push_back(parse_expression());
            expect(TokenType::BRACE_CLOSE);
        }
    }

    return std::make_unique<CommandNode>(
        cmd.Value,
        std::move(collected_args),
        cmd.Info,
        cmd.line,
        cmd.column);
}

/// @brief Parse a factor (higher precedence)
/// @return AST node for factor
/// @note Handles: implicit multiplication (2x, xy)
std::unique_ptr<ASTNode> Parser::parse_factor()
{
    auto left = parse_exponent();

    while (true)
    {
        if (at_end())
            break;

        if (left->Type == ASTNodeType::VARIABLE &&
            match(TokenType::PUNCTUATION) && peek().Value == "(")
        {
            advance();

            std::vector<std::unique_ptr<ASTNode>> args;

            if (!match(TokenType::PUNCTUATION) || peek().Value != ")")
            {
                args.push_back(parse_expression());

                while (match(TokenType::PUNCTUATION) && peek().Value == ",")
                {
                    advance();
                    args.push_back(parse_expression());
                }
            }

            Token close = peek();

            if (close.Type != TokenType::PUNCTUATION || close.Value != ")")
            {
                throw ParseError("Expected closing parenthesis", close.line, close.column);
            }

            advance();

            auto *var = static_cast<VariableNode *>(left.get());

            left = std::make_unique<CommandNode>(
                var->name,
                std::move(args),
                nullptr,
                left->line,
                left->column);

            continue;
        }

        if (!(match(TokenType::NUMBER) ||
              match(TokenType::IDENTIFIER) ||
              match(TokenType::COMMAND) ||
              match(TokenType::BRACE_OPEN)))
        {
            break;
        }

        auto right = parse_exponent();

        left = std::make_unique<BinaryOpNode>(
            '*',
            std::move(left),
            std::move(right),
            left->line,
            left->column);
    }

    return left;
}

/// @brief Parse an exponent
/// @return AST node for exponent
std::unique_ptr<ASTNode> Parser::parse_exponent()
{
    auto left = parse_primary();

    while (match(TokenType::SUPERSCRIPT))
    {
        Token op = advance();
        auto right = parse_primary();

        left = std::make_unique<BinaryOpNode>(
            '^',
            std::move(left),
            std::move(right),
            op.line,
            op.column);
    }

    return left;
}

/// @brief Parse a term (medium precedence)
/// @return AST node for term
/// @note Handles: multiplication, division
std::unique_ptr<ASTNode> Parser::parse_term()
{
    auto left = parse_factor();

    while (!at_end())
    {
        Token op = peek();
        if (op.Type != TokenType::STAR && op.Type != TokenType::SLASH)
            break;

        advance();
        auto right = parse_factor();

        left = std::make_unique<BinaryOpNode>(
            op.Value[0],
            std::move(left),
            std::move(right),
            op.line,
            op.column);
    }

    return left;
}

/// @brief Parse an assignment statement
/// @return AST node for assignment
std::unique_ptr<ASTNode> Parser::parse_assignment()
{
    auto left = parse_expression();

    if (match(TokenType::EQUAL))
    {
        Token eq = advance();
        auto right = parse_assignment();

        return std::make_unique<AssignNode>(
            std::move(left),
            std::move(right),
            eq.line,
            eq.column);
    }

    return left;
}

/// @brief Parse tokens into an AST
/// @return Root node of the AST
/// @throws ParseError if parsing fails
std::unique_ptr<ASTNode> Parser::parse()
{
    if (at_end())
    {
        throw ParseError("Empty input", 0, 0);
    }

    auto ast = parse_assignment();

    if (!at_end() && peek().Type != TokenType::END_OF_FILE)
    {
        Token unexpected = peek();

        throw ParseError(
            "Unexpected token after expression",
            unexpected.line,
            unexpected.column);
    }

    return ast;
}
