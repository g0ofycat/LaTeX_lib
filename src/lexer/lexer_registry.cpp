#include <cctype>
#include <vector>
#include <string>
#include <array>

#include "./lexer.hpp"
#include "./token_info.hpp"
#include "../parser/data/latex_commands.hpp"

// ======================
// -- INIT
// ======================

const std::array<Lexer::LexerAction, 256> Lexer::LEXER_DISPATCH_TABLE = []
{
    std::array<Lexer::LexerAction, 256> table{};

    for (int i = 0; i < 256; i++)
    {
        table[i] = &Lexer::handle_invalid;
    }

    for (char c = '0'; c <= '9'; c++)
    {
        table[static_cast<unsigned char>(c)] = &Lexer::handle_number;
    }

    for (char c = 'a'; c <= 'z'; c++)
    {
        table[static_cast<unsigned char>(c)] = &Lexer::handle_identifier;
    }

    for (char c = 'A'; c <= 'Z'; c++)
    {
        table[static_cast<unsigned char>(c)] = &Lexer::handle_identifier;
    }

    table[(unsigned char)' '] = table[(unsigned char)'\t'] = table[(unsigned char)'\n'] = &Lexer::handle_whitespace;

    table[(unsigned char)'\\'] = &Lexer::handle_command;
    table[(unsigned char)'{'] = &Lexer::handle_brace_open;
    table[(unsigned char)'}'] = &Lexer::handle_brace_close;
    table[(unsigned char)'['] = &Lexer::handle_bracket_open;
    table[(unsigned char)']'] = &Lexer::handle_bracket_close;
    table[(unsigned char)'+'] = &Lexer::handle_plus;
    table[(unsigned char)'-'] = &Lexer::handle_minus;
    table[(unsigned char)'*'] = &Lexer::handle_star;
    table[(unsigned char)'/'] = &Lexer::handle_slash;

    table[(unsigned char)'%'] = &Lexer::handle_comment;

    table[(unsigned char)'^'] = &Lexer::handle_math_op;
    table[(unsigned char)'_'] = &Lexer::handle_math_op;
    table[(unsigned char)'$'] = &Lexer::handle_math_op;
    table[(unsigned char)'&'] = &Lexer::handle_math_op;

    table[(unsigned char)'('] = &Lexer::handle_paren_open;
    table[(unsigned char)')'] = &Lexer::handle_paren_close;
    table[(unsigned char)'\''] = &Lexer::handle_punctuation;
    table[(unsigned char)','] = &Lexer::handle_punctuation;
    table[(unsigned char)'.'] = &Lexer::handle_punctuation;
    table[(unsigned char)':'] = &Lexer::handle_punctuation;
    table[(unsigned char)';'] = &Lexer::handle_punctuation;
    table[(unsigned char)'!'] = &Lexer::handle_punctuation;
    table[(unsigned char)'?'] = &Lexer::handle_punctuation;

    table[(unsigned char)'<'] = &Lexer::handle_less;
    table[(unsigned char)'>'] = &Lexer::handle_greater;
    table[(unsigned char)'!'] = &Lexer::handle_bang;

    table[(unsigned char)'='] = &Lexer::handle_equal;

    return table;
}();

// ======================
// -- METHODS
// ======================

/// @brief Peek at current character without advancing
/// @return Current character or '\0' if at end
char Lexer::peek() const
{
    if (position >= input.size())
        return '\0';

    return input[position];
}

/// @brief Advance position and return current character
/// @return Current character before advancing, or '\0' if at end
char Lexer::advance()
{
    if (position >= input.size())
        return '\0';

    char current = input[position];
    position++;

    if (current == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    return current;
}

/// @brief Read a number
/// @param tokens: The current tokens
void Lexer::handle_number(std::vector<Token> &tokens)
{
    int start_line = line;
    int start_column = column;
    size_t start = position;

    while (std::isdigit(static_cast<unsigned char>(peek())))
    {
        advance();
    }

    if (peek() == '.')
    {
        advance();

        while (std::isdigit(static_cast<unsigned char>(peek())))
        {
            advance();
        }
    }

    tokens.push_back({std::string_view(input.data() + start, position - start),
                      nullptr,
                      TokenType::NUMBER,
                      start_line,
                      start_column});
}

/// @brief Handle a single character
/// @param tokens: The current tokens
/// @param type: Current token type
void Lexer::handle_single_char(std::vector<Token> &tokens, TokenType type)
{
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({std::string_view(input.data() + start, 1),
                      nullptr,
                      type,
                      start_line,
                      start_column});
}

/// @brief Read the current input
/// @param tokens: The current tokens
void Lexer::handle_command(std::vector<Token> &tokens)
{
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    char c = peek();

    if (std::isalpha(static_cast<unsigned char>(c)))
    {
        while (std::isalpha(static_cast<unsigned char>(peek())))
        {
            advance();
        }
    }
    else if (c != '\0')
    {
        advance();
    }

    std::string_view cmd(input.data() + start, position - start);
    const CommandInfo *info = LatexParser::find_command(cmd);

    tokens.push_back({cmd,
                      info,
                      info ? TokenType::COMMAND : TokenType::UNKNOWN,
                      start_line,
                      start_column});
}

/// @brief DISPATCH: PLUS
/// @param tokens: The current tokens
void Lexer::handle_plus(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::PLUS);
}

/// @brief DISPATCH: MINUS
/// @param tokens: The current tokens
void Lexer::handle_minus(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::MINUS);
}

/// @brief DISPATCH: STAR
/// @param tokens: The current tokens
void Lexer::handle_star(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::STAR);
}

/// @brief DISPATCH: SLASH
/// @param tokens: The current tokens
void Lexer::handle_slash(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::SLASH);
}

/// @brief DISPATCH: BRACE (OPEN)
/// @param tokens: The current tokens
void Lexer::handle_brace_open(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::BRACE_OPEN);
}

/// @brief DISPATCH: BRACE (CLOSED)
/// @param tokens: The current tokens
void Lexer::handle_brace_close(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::BRACE_CLOSE);
}

/// @brief DISPATCH: BRACKET (OPEN)
/// @param tokens: The current tokens
void Lexer::handle_bracket_open(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::BRACKET_OPEN);
}

/// @brief DISPATCH: BRACKET (CLOSED)
/// @param tokens: The current tokens
void Lexer::handle_bracket_close(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::BRACKET_CLOSE);
}

/// @brief DISPATCH: PAREN (OPEN)
/// @param tokens: The current tokens
void Lexer::handle_paren_open(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::PAREN_OPEN);
}

/// @brief DISPATCH: PAREN (CLOSE)
/// @param tokens: The current tokens
void Lexer::handle_paren_close(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::PAREN_CLOSE);
}

/// @brief DISPATCH: PUNCTUATION
/// @param tokens: The current tokens
void Lexer::handle_punctuation(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::PUNCTUATION);
}

/// @brief DISPATCH: EQUAL
/// @param tokens: The current tokens
void Lexer::handle_equal(std::vector<Token> &tokens)
{
    handle_single_char(tokens, TokenType::EQUAL);
}

/// @brief DISPATCH: <
/// @param tokens: The current tokens
void Lexer::handle_less(std::vector<Token> &tokens)
{
    int s_line = line;
    int s_col = column;
    size_t s_pos = position;
    advance();

    if (peek() == '=')
    {
        advance();
        tokens.push_back({std::string_view(input.data() + s_pos, 2),
                          nullptr, TokenType::LESS_EQUAL, s_line, s_col});
    }
    else
    {
        tokens.push_back({std::string_view(input.data() + s_pos, 1),
                          nullptr, TokenType::LESS, s_line, s_col});
    }
}

/// @brief DISPATCH: >
/// @param tokens: The current tokens
void Lexer::handle_greater(std::vector<Token> &tokens)
{
    int s_line = line;
    int s_col = column;
    size_t s_pos = position;
    advance();

    if (peek() == '=')
    {
        advance();
        tokens.push_back({std::string_view(input.data() + s_pos, 2),
                          nullptr, TokenType::GREATER_EQUAL, s_line, s_col});
    }
    else
    {
        tokens.push_back({std::string_view(input.data() + s_pos, 1),
                          nullptr, TokenType::GREATER, s_line, s_col});
    }
}

/// @brief DISPATCH: !
/// @param tokens: The current tokens
void Lexer::handle_bang(std::vector<Token> &tokens)
{
    int s_line = line;
    int s_col = column;
    size_t s_pos = position;
    advance();

    if (peek() == '=')
    {
        advance();
        tokens.push_back({std::string_view(input.data() + s_pos, 2),
                          nullptr, TokenType::NOT_EQUAL, s_line, s_col});
    }
    else
    {
        tokens.push_back({std::string_view(input.data() + s_pos, 1),
                          nullptr, TokenType::PUNCTUATION, s_line, s_col});
    }
}

/// @brief DISPATCH: COMMENT
/// @param tokens: The current tokens
void Lexer::handle_comment(std::vector<Token> &tokens)
{
    while (peek() != '\n' && peek() != '\0')
    {
        advance();
    }

    if (peek() == '\n')
    {
        advance();
    }
}

/// @brief DISPATCH: WHITESPACE
/// @param tokens: The current tokens
void Lexer::handle_whitespace(std::vector<Token> &tokens)
{
    while (peek() == ' ' || peek() == '\t' || peek() == '\n')
    {
        advance();
    }
}

/// @brief DISPATCH: INVALID
/// @param tokens: The current tokens
void Lexer::handle_invalid(std::vector<Token> &tokens)
{
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({std::string_view(input.data() + start, 1),
                      nullptr,
                      TokenType::INVALID,
                      start_line,
                      start_column});
}

/// @brief DISPATCH: IDENTIFIER
/// @param tokens: The current tokens
void Lexer::handle_identifier(std::vector<Token> &tokens)
{
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    while (std::isalnum(static_cast<unsigned char>(peek())))
    {
        advance();
    }

    tokens.push_back({std::string_view(input.data() + start, position - start),
                      nullptr,
                      TokenType::IDENTIFIER,
                      start_line,
                      start_column});
}

/// @brief DISPATCH: MATH DELIMITERS
/// @param tokens: The current tokens
void Lexer::handle_math_op(std::vector<Token> &tokens)
{
    char c = peek();

    TokenType type;

    switch (c)
    {
    case '^':
        type = TokenType::SUPERSCRIPT;
        break;
    case '_':
        type = TokenType::SUBSCRIPT;
        break;
    case '$':
        type = TokenType::DOLLAR;
        break;
    case '&':
        type = TokenType::ALIGNMENT;
        break;
    default:
        type = TokenType::INVALID;
        break;
    }

    handle_single_char(tokens, type);
}

/// @brief Tokenize the current input
/// @return std::vector<Token>
std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    tokens.reserve(input.size() / 4);

    while (position < input.size())
    {
        unsigned char current = static_cast<unsigned char>(peek());
        LexerAction action = LEXER_DISPATCH_TABLE[current];
        (this->*action)(tokens);
    }

    tokens.push_back({std::string_view(input.data() + position, 0),
                      nullptr,
                      TokenType::END_OF_FILE,
                      line,
                      column});

    return tokens;
}