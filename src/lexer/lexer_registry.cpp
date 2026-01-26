#include <cctype>
#include <vector>
#include <string>
#include <array>
#include <functional>

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
        table[i] = [](Lexer *s, std::vector<Token> &t)
        { s->handle_invalid(t); };

    auto map_char = [&](unsigned char c, TokenType type)
    {
        table[c] = [type](Lexer *self, std::vector<Token> &tokens)
        {
            self->handle_single_char(tokens, type);
        };
    };

    auto num = [](Lexer *s, std::vector<Token> &t)
    { s->handle_number(t); };
    for (char c = '0'; c <= '9'; c++)
        table[(unsigned char)c] = num;

    auto ident = [](Lexer *s, std::vector<Token> &t)
    { s->handle_identifier(t); };
    for (char c = 'a'; c <= 'z'; c++)
        table[(unsigned char)c] = ident;
    for (char c = 'A'; c <= 'Z'; c++)
        table[(unsigned char)c] = ident;

    table[(unsigned char)' '] = table[(unsigned char)'\t'] =
        table[(unsigned char)'\n'] = [](Lexer *s, std::vector<Token> &t)
    { s->handle_whitespace(t); };

    table[(unsigned char)'\\'] = [](Lexer *s, std::vector<Token> &t)
    { s->handle_command(t); };
    table[(unsigned char)'<'] = [](Lexer *s, std::vector<Token> &t)
    { s->handle_less(t); };
    table[(unsigned char)'>'] = [](Lexer *s, std::vector<Token> &t)
    { s->handle_greater(t); };
    table[(unsigned char)'%'] = [](Lexer *s, std::vector<Token> &t)
    { s->handle_comment(t); };

    map_char('{', TokenType::BRACE_OPEN);
    map_char('}', TokenType::BRACE_CLOSE);

    map_char('(', TokenType::PAREN_OPEN);
    map_char(')', TokenType::PAREN_CLOSE);

    map_char('[', TokenType::BRACKET_OPEN);
    map_char(']', TokenType::BRACKET_CLOSE);

    map_char('+', TokenType::PLUS);
    map_char('-', TokenType::MINUS);
    map_char('*', TokenType::STAR);
    map_char('/', TokenType::SLASH);

    map_char('^', TokenType::SUPERSCRIPT);
    map_char('_', TokenType::SUBSCRIPT);
    map_char('&', TokenType::ALIGNMENT);
    map_char('$', TokenType::DOLLAR);

    map_char('\'', TokenType::PUNCTUATION);
    map_char('.', TokenType::PUNCTUATION);
    map_char(':', TokenType::PUNCTUATION);
    map_char(';', TokenType::PUNCTUATION);
    map_char('?', TokenType::PUNCTUATION);

    map_char(',', TokenType::SPACING);

    map_char('=', TokenType::EQUAL);
    map_char('!', TokenType::FACTORIAL);

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
    size_t start_pos = position;

    advance();

    char c = peek();

    if (c != '\0' && !std::isalpha(static_cast<unsigned char>(c)))
    {
        advance();
        std::string_view cmd(input.data() + start_pos, position - start_pos);

        if (const CommandInfo *info = LatexParser::find_command(cmd))
            tokens.push_back({cmd, info, info->type_override, start_line, start_column});
        else
            tokens.push_back({cmd, nullptr, TokenType::COMMAND, start_line, start_column});

        return;
    }

    while (std::isalpha(static_cast<unsigned char>(peek())))
        advance();

    std::string_view cmd(input.data() + start_pos, position - start_pos);

    const CommandInfo *info = LatexParser::find_command(cmd);
    TokenType type = info ? info->type_override : TokenType::COMMAND;

    tokens.push_back({cmd, info, type, start_line, start_column});
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

    while (std::isalpha(static_cast<unsigned char>(peek())))
    {
        advance();
    }

    tokens.push_back({std::string_view(input.data() + start, position - start),
                      nullptr,
                      TokenType::IDENTIFIER,
                      start_line,
                      start_column});
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
        action(this, tokens);
    }

    tokens.push_back({std::string_view(input.data() + position, 0),
                      nullptr,
                      TokenType::END_OF_FILE,
                      line,
                      column});

    return tokens;
}
