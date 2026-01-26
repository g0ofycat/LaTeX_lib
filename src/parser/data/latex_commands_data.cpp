#include <unordered_map>

#include "./latex_commands.hpp"

// ======================
// -- INIT
// ======================

namespace LatexParser
{
    const std::unordered_map<std::string_view, CommandInfo> LATEX_COMMANDS = {

        // ======================
        // -- GREEK LETTERS
        // ======================

        {"\\alpha", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\beta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\gamma", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\delta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\epsilon", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\varepsilon", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\zeta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\eta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\theta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\vartheta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\iota", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\kappa", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\lambda", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\mu", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\nu", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\xi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\pi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\rho", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\sigma", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\varsigma", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\tau", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\upsilon", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\phi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\varphi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\chi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\psi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\omega", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        {"\\Gamma", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Delta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Theta", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Lambda", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Xi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Pi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Sigma", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Upsilon", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Phi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Psi", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Omega", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- TRIG
        // ======================

        {"\\sin", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\cos", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\tan", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\csc", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\sec", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\cot", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\sinh", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\cosh", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\tanh", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\arcsin", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\arccos", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\arctan", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},

        // ======================
        // -- CALC / OPERATORS
        // ======================

        {"\\lim", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\sup", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\inf", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\max", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\min", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\log", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\ln", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\exp", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\det", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\dim", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\ker", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},
        {"\\gcd", {CommandType::MATH, TokenType::COMMAND, 1, 0, true, true}},

        {"\\partial", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- INTERGRALS / SUMS / PRODUCTS
        // ======================

        {"\\int", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\iint", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\iiint", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\oint", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\sum", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\prod", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\bigcup", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},
        {"\\bigcap", {CommandType::MATH, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- FRACTIONS / ROOTS / MODIFIERS
        // ======================

        {"\\frac", {CommandType::MATH, TokenType::COMMAND, 2, 0, true, true}},
        {"\\binom", {CommandType::MATH, TokenType::COMMAND, 2, 0, true, true}},
        {"\\choose", {CommandType::MATH, TokenType::COMMAND, 2, 0, true, true}},
        {"\\sqrt", {CommandType::MATH, TokenType::COMMAND, 1, 1, true, true}},
        {"\\bar", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\hat", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\tilde", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\dot", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\ddot", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\vec", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\overline", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\underline", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\overbrace", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\underbrace", {CommandType::ACCENT, TokenType::COMMAND, 1, 0, true, true}},

        // ======================
        // -- RELATIONS / LOGIC
        // ======================

        {"\\leq", {CommandType::SYMBOL, TokenType::LESS_EQUAL, 0, 0, true, true}},
        {"\\geq", {CommandType::SYMBOL, TokenType::GREATER_EQUAL, 0, 0, true, true}},
        {"\\neq", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\equiv", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\approx", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\sim", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\cong", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\propto", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\prec", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\succ", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\subset", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\subseteq", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\supset", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\supseteq", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\in", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\notin", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\forall", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\exists", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\neg", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\land", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\lor", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\implies", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\iff", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- ARROWS
        // ======================

        {"\\leftarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\rightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\leftrightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Leftarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Rightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Leftrightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\uparrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\downarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Uparrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\Downarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\hookleftarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\hookrightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\mapsto", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\longleftarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\longrightarrow", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- DELIMS / GEOMETRY
        // ======================

        {"\\langle", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\rangle", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\lfloor", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\rfloor", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\lceil", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\rceil", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\angle", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\measuredangle", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\triangle", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\square", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\perp", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\parallel", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- DOTS / MISC SYMBOLS
        // ======================

        {"\\cdot", {CommandType::SYMBOL, TokenType::STAR, 0, 0, true, true}},
        {"\\times", {CommandType::SYMBOL, TokenType::STAR, 0, 0, true, true}},
        {"\\pm", {CommandType::SYMBOL, TokenType::PLUS_MINUS, 0, 0, true, true}},
        {"\\mp", {CommandType::SYMBOL, TokenType::MINUS_PLUS, 0, 0, true, true}},
        {"\\div", {CommandType::SYMBOL, TokenType::SLASH, 0, 0, true, true}},
        {"\\star", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\bullet", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\dots", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\cdots", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\ldots", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\vdots", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\ddots", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\infty", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\emptyset", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\aleph", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\hbar", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\wp", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},
        {"\\nabla", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        // ======================
        // -- FONTS / TEXT
        // ======================

        {"\\text", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathrm", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathbb", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathbf", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathcal", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathfrak", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\mathit", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\operatorname", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},

        // ======================
        // -- ENV / STRUCTURES
        // ======================

        {"\\left", {CommandType::PREFIX_DELIMITER, TokenType::LEFT_WRAP, 0, 0, true, true}},
        {"\\right", {CommandType::POSTFIX_DELIMITER, TokenType::RIGHT_WRAP, 0, 0, true, true}},

        {"\\begin", {CommandType::PREFIX_DELIMITER, TokenType::ENV_BEGIN, 1, 0, true, true}},
        {"\\end", {CommandType::POSTFIX_DELIMITER, TokenType::ENV_END, 1, 0, true, true}},

        {"\\cases", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\matrix", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\pmatrix", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\bmatrix", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\vmatrix", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},
        {"\\Vmatrix", {CommandType::TEXT, TokenType::COMMAND, 1, 0, true, true}},

        // ======================
        // -- MISC
        // ======================

        {"\\to", {CommandType::SYMBOL, TokenType::COMMAND, 0, 0, true, true}},

        {"\\[", {CommandType::SYMBOL, TokenType::DISPLAY_MATH_OPEN, 0, 0, true, true}},
        {"\\]", {CommandType::SYMBOL, TokenType::DISPLAY_MATH_CLOSE, 0, 0, true, true}},
        {"\\(", {CommandType::SYMBOL, TokenType::INLINE_MATH_OPEN, 0, 0, true, true}},
        {"\\)", {CommandType::SYMBOL, TokenType::INLINE_MATH_CLOSE, 0, 0, true, true}},
        {"\\{", {CommandType::SYMBOL, TokenType::ESCAPED_BRACE_OPEN, 0, 0, true, true}},
        {"\\}", {CommandType::SYMBOL, TokenType::ESCAPED_BRACE_CLOSE, 0, 0, true, true}},
        {R"(\\)", {CommandType::SYMBOL, TokenType::NEWLINE, 0, 0, true, true}},
    };
}
