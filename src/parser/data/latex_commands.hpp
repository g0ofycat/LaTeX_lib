#ifndef LATEX_COMMANDS_HPP
#define LATEX_COMMANDS_HPP

#include <string_view>
#include <vector>

#include "./latex_info.hpp"

// ======================
// -- NAMESPACES
// ======================

namespace LatexParser {
    extern const std::vector<CommandInfo> LATEX_COMMANDS;

    /// @brief Find the CommandType for a specific CommandType
    /// @param name: The name of the command
    /// @return const CommandInfo*
    const CommandInfo* find_command(const std::string_view name);
}

#endif
