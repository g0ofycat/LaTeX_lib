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

const std::array<Lexer::LexerAction, 256> Lexer::DISPATCH_TABLE = [] {
    std::array<Lexer::LexerAction, 256> table{};

    for (int i = 0; i < 256; i++) {
        table[i] = &Lexer::handle_invalid;
    }

    for (char c = '0'; c <= '9'; c++) {
        table[static_cast<unsigned char>(c)] = &Lexer::handle_number;
    }

    for (char c = 'a'; c <= 'z'; c++) {
                table[static_cast<unsigned char>(c)] = &Lexer::handle_identifier;
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        table[static_cast<unsigned char>(c)] = &Lexer::handle_identifier;
    }

    table[static_cast<unsigned char>('\\')] = &Lexer::handle_command;
    table[static_cast<unsigned char>('+')] = &Lexer::handle_plus;
    table[static_cast<unsigned char>('-')] = &Lexer::handle_minus;
    table[static_cast<unsigned char>('{')] = &Lexer::handle_brace_open;
    table[static_cast<unsigned char>('}')] = &Lexer::handle_brace_close;
    table[static_cast<unsigned char>(' ')] = &Lexer::handle_whitespace;
    table[static_cast<unsigned char>('\t')] = &Lexer::handle_whitespace;
    table[static_cast<unsigned char>('\n')] = &Lexer::handle_whitespace;

    return table;
}();

// ======================
// -- METHODS
// ======================

/// @brief Peek at current character without advancing
/// @return Current character or '\0' if at end
char Lexer::peek() const {
    if (position >= input.size()) return '\0';

    return input[position];
}

/// @brief Advance position and return current character
/// @return Current character before advancing, or '\0' if at end
char Lexer::advance() {
    if (position >= input.size()) return '\0';

    char current = input[position];
    position++;

    if (current == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }

    return current;
}

/// @brief Read a number
/// @param tokens: The current tokens
void Lexer::handle_number(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }

    if (peek() == '.') {
        advance();

        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    tokens.push_back({
        std::string_view(input.data() + start, position - start),
        nullptr,
        TokenType::NUMBER,
        start_line,
        start_column
    });
}

/// @brief Read the current input
/// @param tokens: The current tokens
void Lexer::handle_command(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    while (std::isalpha(static_cast<unsigned char>(peek()))) {
        advance();
    }

    std::string_view cmd(input.data() + start, position - start);
    const CommandInfo* info = LatexParser:: find_command(cmd);

    tokens.push_back({
        cmd,
        info,
        info ? TokenType::COMMAND : TokenType::UNKNOWN,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: PLUS
/// @param tokens: The current tokens
void Lexer::handle_plus(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::PLUS,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: MINUS
/// @param tokens: The current tokens
void Lexer::handle_minus(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::MINUS,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: BRACE (OPEN)
/// @param tokens: The current tokens
void Lexer::handle_brace_open(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::BRACE_OPEN,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: BRACE (CLOSE)
/// @param tokens: The current tokens
void Lexer::handle_brace_close(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::BRACE_CLOSE,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: WHITESPACE
/// @param tokens: The current tokens
void Lexer::handle_whitespace(std::vector<Token>& tokens) {
    while (peek() == ' ' || peek() == '\t' || peek() == '\n') {
        advance();
    }
}

/// @brief DISPATCH: INVALID
/// @param tokens: The current tokens
void Lexer::handle_invalid(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::INVALID,
        start_line,
        start_column
    });
}

/// @brief DISPATCH: IDENTIFIER
/// @param tokens: The current tokens
void Lexer::handle_identifier(std::vector<Token>& tokens) {
    int start_line = line;
    int start_column = column;
    size_t start = position;

    advance();

    tokens.push_back({
        std::string_view(input.data() + start, 1),
        nullptr,
        TokenType::IDENTIFIER,
        start_line,
        start_column
    });
}

/// @brief Tokenize the current input
/// @return std::vector<Token>
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    tokens.reserve(input.size() / 2);

    while (position < input.size()) {
        unsigned char current = static_cast<unsigned char>(peek());
        LexerAction action = DISPATCH_TABLE[current];
        (this->*action)(tokens);
    }

    tokens.push_back({
        std::string_view(),
        nullptr,
        TokenType::END_OF_FILE,
        line,
        column
    });

    return tokens;
}
