#ifndef LATEX_INFO_HPP
#define LATEX_INFO_HPP

#include <string>
#include <vector>

// ======================
// -- ENUMS / STRUCTS
// ======================

enum class CommandType
{
    SYMBOL,
    UNARY,
    BINARY
};

enum class ArgType
{
    REQUIRED,
    OPTIONAL
};

struct CommandInfo
{
    CommandType Type;                 // CommandType: Type of command
    int Precedence;                   // int: Order of Operations
    std::vector<ArgType> arg_pattern; // std::vector<ArgType>: Pattern of arguments
};

#endif
