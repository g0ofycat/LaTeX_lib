#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <array>

#include "./token_info.hpp"

// ======================
// -- LEXER 
// ======================

class Lexer {
    private:
        // ======================
        // -- LEXER DATA
        // ======================

        std::string input;
        size_t position{0};

        int line{0};
        int column{1};

        // ======================
        // -- DISPATCH DATA
        // ======================

        using LexerAction = void (Lexer::*)(std::vector<Token>&);

        static const std::array<LexerAction, 256> DISPATCH_TABLE;

    private:
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
        void handle_number(std::vector<Token>& tokens);

        /// @brief Read the current input
        /// @param tokens: The current tokens
        void handle_command(std::vector<Token>& tokens);

        // ======================
        // -- DISPATCH METHODS
        // ======================

        /// @brief DISPATCH: PLUS
        /// @param tokens: The current tokens
        void handle_plus(std::vector<Token>& tokens);

        /// @brief DISPATCH: MINUS
        /// @param tokens: The current tokens
        void handle_minus(std::vector<Token>& tokens);

        /// @brief DISPATCH: BRACE (OPEN)
        /// @param tokens: The current tokens
        void handle_brace_open(std::vector<Token>& tokens);

        /// @brief DISPATCH: BRACE (CLOSE)
        /// @param tokens: The current tokens
        void handle_brace_close(std::vector<Token>& tokens);

        /// @brief DISPATCH: BRACKET (OPEN)
        /// @param tokens: The current tokens
        void handle_bracket_open(std::vector<Token>& tokens);

        /// @brief DISPATCH: BRACKET (CLOSED)
        /// @param tokens: The current tokens
        void handle_bracket_close(std::vector<Token>& tokens);

        /// @brief DISPATCH: COMMENT
        /// @param tokens: The current tokens
        void handle_comment(std::vector<Token>& tokens);

        /// @brief DISPATCH: WHITESPACE
        /// @param tokens: The current tokens
        void handle_whitespace(std::vector<Token>& tokens);

        /// @brief DISPATCH: INVALID
        /// @param tokens: The current tokens
        void handle_invalid(std::vector<Token>& tokens);

        /// @brief DISPATCH: IDENTIFIER (variables like x, y, z)
        /// @param tokens: The current tokens
        void handle_identifier(std::vector<Token>& tokens);

        /// @brief DISPATCH: MATH DELIMITERS
        /// @param tokens: The current tokens
        void handle_math_op(std::vector<Token>& tokens);

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
