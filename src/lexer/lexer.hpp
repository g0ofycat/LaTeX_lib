#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <array>
#include <functional>

#include "./token_info.hpp"

// ======================
// -- LEXER
// ======================

class Lexer
{
private:
    // ======================
    // -- LEXER DATA
    // ======================

    std::string input;
    size_t position{0};

    int line{1};
    int column{1};

    // ======================
    // -- DISPATCH DATA
    // ======================

    using LexerAction = std::function<void(Lexer *, std::vector<Token> &)>;

    static const std::array<LexerAction, 256> LEXER_DISPATCH_TABLE;

    // ======================
    // -- LEXER UTILITY
    // ======================

    /// @brief Peek at current character without advancing
    /// @return Current character or '\0' if at end
    char peek() const;

    /// @brief Advance position and return current character
    /// @return Current character before advancing, or '\0' if at end
    char advance();

    /// @brief Read a number
    /// @param tokens: The current tokens
    void handle_number(std::vector<Token> &tokens);

    /// @brief Handle a single character
    /// @param tokens: The current tokens
    /// @param type: Current token type
    void handle_single_char(std::vector<Token> &tokens, TokenType type);

    /// @brief Read the current input
    /// @param tokens: The current tokens
    void handle_command(std::vector<Token> &tokens);

    // ======================
    // -- DISPATCH METHODS
    // ======================

    /// @brief DISPATCH: <
    /// @param tokens: The current tokens
    void handle_less(std::vector<Token> &tokens);

    /// @brief DISPATCH: >
    /// @param tokens: The current tokens
    void handle_greater(std::vector<Token> &tokens);

    /// @brief DISPATCH: COMMENT
    /// @param tokens: The current tokens
    void handle_comment(std::vector<Token> &tokens);

    /// @brief DISPATCH: WHITESPACE
    /// @param tokens: The current tokens
    void handle_whitespace(std::vector<Token> &tokens);

    /// @brief DISPATCH: INVALID
    /// @param tokens: The current tokens
    void handle_invalid(std::vector<Token> &tokens);

    /// @brief DISPATCH: IDENTIFIER (variables like x, y, z)
    /// @param tokens: The current tokens
    void handle_identifier(std::vector<Token> &tokens);

public:
    // ======================
    // -- CONSTRUCTOR
    // ======================

    /// @brief Lexer Constructor
    /// @param text: The text to put into the lexer
    Lexer(std::string text) : input(std::move(text)) {}

    // ======================
    // -- PUBLIC METHODS
    // ======================

    /// @brief Tokenize the current input
    /// @return std::vector<Token>
    std::vector<Token> tokenize();
};

#endif
