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
        auto it = std::lower_bound(LATEX_COMMANDS.begin(), LATEX_COMMANDS.end(), name,
                                   [](const CommandInfo &info, const std::string_view val)
                                   {
                                       return info.LaTeX < val;
                                   });

        if (it != LATEX_COMMANDS.end() && it->LaTeX == name)
            return &(*it);

        return nullptr;
    }
}
