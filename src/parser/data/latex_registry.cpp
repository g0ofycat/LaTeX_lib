#include <vector>
#include <string_view>
#include <algorithm>

#include "./latex_commands.hpp"

#include "./latex_info.hpp"

// ======================
// -- INIT
// ======================

namespace LatexParser
{
    /// @brief Find the CommandType for a specific CommandType
    /// @param name: The name of the command
    /// @return const CommandInfo
    const CommandInfo *find_command(std::string_view name)
    {
        auto it = LATEX_COMMANDS.find(name);

        return it != LATEX_COMMANDS.end() ? &it->second : nullptr;
    }
}
