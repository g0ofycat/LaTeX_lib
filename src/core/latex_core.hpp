#ifndef LATEX_CORE_HPP
#define LATEX_CORE_HPP

#include <string>
#include <vector>

#include "../sem_analyzer/semantic_analyzer.hpp"

class LatexCore
{
public:
    // ======================
    // -- STATIC API
    // ======================

    /// @brief Process a string and return analysis results
    /// @param text: The text to process
    /// @return std::vector<SemanticError>
    static std::vector<SemanticError> analyze(const std::string &text);
};

#endif
