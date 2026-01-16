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

        {"\\alpha", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\beta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\gamma", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\delta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\epsilon", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\varepsilon", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\zeta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\eta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\theta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\vartheta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\iota", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\kappa", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\lambda", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\mu", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\nu", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\xi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\pi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\rho", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\sigma", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\varsigma", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\tau", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\upsilon", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\phi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\varphi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\chi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\psi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\omega", {CommandType::SYMBOL, 0, 0, true, true}},

        {"\\Gamma", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Delta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Theta", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Lambda", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Xi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Pi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Sigma", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Upsilon", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Phi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Psi", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Omega", {CommandType::SYMBOL, 0, 0, true, true}},

        // ======================
        // -- TRIG
        // ======================

        {"\\sin", {CommandType::MATH, 0, 0, true, true}},
        {"\\cos", {CommandType::MATH, 0, 0, true, true}},
        {"\\tan", {CommandType::MATH, 1, 0, true, true}},
        {"\\csc", {CommandType::MATH, 1, 0, true, true}},
        {"\\sec", {CommandType::MATH, 1, 0, true, true}},
        {"\\cot", {CommandType::MATH, 1, 0, true, true}},
        {"\\sinh", {CommandType::MATH, 1, 0, true, true}},
        {"\\cosh", {CommandType::MATH, 1, 0, true, true}},
        {"\\tanh", {CommandType::MATH, 1, 0, true, true}},
        {"\\arcsin", {CommandType::MATH, 1, 0, true, true}},
        {"\\arccos", {CommandType::MATH, 1, 0, true, true}},
        {"\\arctan", {CommandType::MATH, 1, 0, true, true}},

        // ======================
        // -- CALC / OPERATORS
        // ======================

        {"\\lim", {CommandType::MATH, 0, 0, true, true}},
        {"\\sup", {CommandType::MATH, 0, 0, true, true}},
        {"\\inf", {CommandType::MATH, 0, 0, true, true}},
        {"\\max", {CommandType::MATH, 0, 0, true, true}},
        {"\\min", {CommandType::MATH, 0, 0, true, true}},
        {"\\log", {CommandType::MATH, 0, 0, true, true}},
        {"\\ln", {CommandType::MATH, 0, 0, true, true}},
        {"\\exp", {CommandType::MATH, 0, 0, true, true}},
        {"\\det", {CommandType::MATH, 1, 0, true, true}},
        {"\\dim", {CommandType::MATH, 1, 0, true, true}},
        {"\\ker", {CommandType::MATH, 1, 0, true, true}},
        {"\\gcd", {CommandType::MATH, 1, 0, true, true}},

        // ======================
        // -- INTERGRALS / SUMS / PRODUCTS
        // ======================

        {"\\int", {CommandType::MATH, 0, 0, true, true}},
        {"\\iint", {CommandType::MATH, 0, 0, true, true}},
        {"\\iiint", {CommandType::MATH, 0, 0, true, true}},
        {"\\oint", {CommandType::MATH, 0, 0, true, true}},
        {"\\sum", {CommandType::MATH, 0, 0, true, true}},
        {"\\prod", {CommandType::MATH, 0, 0, true, true}},
        {"\\bigcup", {CommandType::MATH, 0, 0, true, true}},
        {"\\bigcap", {CommandType::MATH, 0, 0, true, true}},

        // ======================
        // -- FRACTIONS / ROOTS / MODIFIERS
        // ======================

        {"\\frac", {CommandType::MATH, 2, 0, true, true}},
        {"\\binom", {CommandType::MATH, 2, 0, true, true}},
        {"\\choose", {CommandType::MATH, 2, 0, true, true}},
        {"\\sqrt", {CommandType::MATH, 1, 1, true, true}},
        {"\\bar", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\hat", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\tilde", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\dot", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\ddot", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\vec", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\overline", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\underline", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\overbrace", {CommandType::ACCENT, 1, 0, true, true}},
        {"\\underbrace", {CommandType::ACCENT, 1, 0, true, true}},

        // ======================
        // -- RELATIONS / LOGIC
        // ======================

        {"\\leq", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\geq", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\neq", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\equiv", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\approx", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\sim", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\cong", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\propto", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\prec", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\succ", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\subset", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\subseteq", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\supset", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\supseteq", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\in", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\notin", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\forall", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\exists", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\neg", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\land", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\lor", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\implies", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\iff", {CommandType::SYMBOL, 0, 0, true, true}},

        // ======================
        // -- ARROWS
        // ======================

        {"\\leftarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\rightarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\leftrightarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Leftarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Rightarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Leftrightarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\uparrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\downarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Uparrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\Downarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\hookleftarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\hookrightarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\mapsto", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\longleftarrow", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\longrightarrow", {CommandType::SYMBOL, 0, 0, true, true}},

        // ======================
        // -- DELIMS / GEOMETRY
        // ======================

        {"\\langle", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\rangle", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\lfloor", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\rfloor", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\lceil", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\rceil", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\angle", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\measuredangle", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\triangle", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\square", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\perp", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\parallel", {CommandType::SYMBOL, 0, 0, true, true}},

        // ======================
        // -- DOTS / MISC SYMBOLS
        // ======================

        {"\\cdot", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\times", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\pm", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\mp", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\div", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\star", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\bullet", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\dots", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\cdots", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\ldots", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\vdots", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\ddots", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\infty", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\emptyset", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\aleph", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\hbar", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\wp", {CommandType::SYMBOL, 0, 0, true, true}},

        // ======================
        // -- FONTS / TEXT
        // ======================

        {"\\text", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathrm", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathbb", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathbf", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathcal", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathfrak", {CommandType::TEXT, 1, 0, true, true}},
        {"\\mathit", {CommandType::TEXT, 1, 0, true, true}},
        {"\\operatorname", {CommandType::TEXT, 1, 0, true, true}},

        // ======================
        // -- ENV / STRUCTURES
        // ======================

        {"\\left", {CommandType::PREFIX_DELIMITER, 0, 0, true, true}},
        {"\\right", {CommandType::POSTFIX_DELIMITER, 0, 0, true, true}},

        {"\\begin", {CommandType::TEXT, 1, 0, true, true}},
        {"\\end", {CommandType::TEXT, 1, 0, true, true}},

        {"\\cases", {CommandType::TEXT, 1, 0, true, true}},
        {"\\matrix", {CommandType::TEXT, 1, 0, true, true}},
        {"\\pmatrix", {CommandType::TEXT, 1, 0, true, true}},
        {"\\bmatrix", {CommandType::TEXT, 1, 0, true, true}},
        {"\\vmatrix", {CommandType::TEXT, 1, 0, true, true}},
        {"\\Vmatrix", {CommandType::TEXT, 1, 0, true, true}},

        // ======================
        // -- MISC
        // ======================

        {"\\to", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\{", {CommandType::SYMBOL, 0, 0, true, true}},
        {"\\}", {CommandType::SYMBOL, 0, 0, true, true}},
    };
}
