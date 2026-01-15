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
    BINARY,
    PREFIX_DELIMITER,
    POSTFIX_DELIMITER,
    MATH,
    TEXT,
    ACCENT,
    UNKNOWN
};

enum class ArgType
{
    REQUIRED,
    OPTIONAL
};

struct CommandInfo
{
    CommandType type = CommandType::UNKNOWN; // CommandType: Type of command
    int mandatory_args = 0;                  // int: # of mandatory args
    int optional_args = 0;                   // int: # of optional args
    bool allows_subscript = true;            // bool: Allow subscripts
    bool allows_superscript = true;          // bool: Allow superscripts
};

#endif
