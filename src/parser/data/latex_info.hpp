#ifndef LATEX_INFO_HPP
#define LATEX_INFO_HPP

#include <string>

// ======================
// -- ENUMS / STRUCTS
// ======================

enum class CommandType { SYMBOL, UNARY, BINARY };

struct CommandInfo {
    std::string LaTeX; // std::string: String Command
    CommandType Type; // CommandType: Type of command
    int Precedence; // int: Order of Operations
    int Args; // int: Number of expected arguments
};

#endif
