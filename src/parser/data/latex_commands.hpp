#ifndef LATEX_COMMANDS_HPP
#define LATEX_COMMANDS_HPP

#include <string_view>
#include <vector>
#include <unordered_map>

#include "./latex_info.hpp"

// ======================
// -- NAMESPACES
// ======================

namespace LatexParser
{
    extern const std::unordered_map<std::string_view, CommandInfo> LATEX_COMMANDS;

    /// @brief Find the CommandType for a specific CommandType
    /// @param name: The name of the command
    /// @return const CommandInfo*
    const CommandInfo *find_command(const std::string_view name);
}

#endif
