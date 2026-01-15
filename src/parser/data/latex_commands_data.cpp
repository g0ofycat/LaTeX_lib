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

        {"\\alpha", {CommandType::SYMBOL, 0, {}}},
        {"\\beta", {CommandType::SYMBOL, 0, {}}},
        {"\\gamma", {CommandType::SYMBOL, 0, {}}},
        {"\\delta", {CommandType::SYMBOL, 0, {}}},
        {"\\epsilon", {CommandType::SYMBOL, 0, {}}},
        {"\\varepsilon", {CommandType::SYMBOL, 0, {}}},
        {"\\zeta", {CommandType::SYMBOL, 0, {}}},
        {"\\eta", {CommandType::SYMBOL, 0, {}}},
        {"\\theta", {CommandType::SYMBOL, 0, {}}},
        {"\\vartheta", {CommandType::SYMBOL, 0, {}}},
        {"\\iota", {CommandType::SYMBOL, 0, {}}},
        {"\\kappa", {CommandType::SYMBOL, 0, {}}},
        {"\\lambda", {CommandType::SYMBOL, 0, {}}},
        {"\\mu", {CommandType::SYMBOL, 0, {}}},
        {"\\nu", {CommandType::SYMBOL, 0, {}}},
        {"\\xi", {CommandType::SYMBOL, 0, {}}},
        {"\\pi", {CommandType::SYMBOL, 0, {}}},
        {"\\rho", {CommandType::SYMBOL, 0, {}}},
        {"\\sigma", {CommandType::SYMBOL, 0, {}}},
        {"\\varsigma", {CommandType::SYMBOL, 0, {}}},
        {"\\tau", {CommandType::SYMBOL, 0, {}}},
        {"\\upsilon", {CommandType::SYMBOL, 0, {}}},
        {"\\phi", {CommandType::SYMBOL, 0, {}}},
        {"\\varphi", {CommandType::SYMBOL, 0, {}}},
        {"\\chi", {CommandType::SYMBOL, 0, {}}},
        {"\\psi", {CommandType::SYMBOL, 0, {}}},
        {"\\omega", {CommandType::SYMBOL, 0, {}}},

        {"\\Gamma", {CommandType::SYMBOL, 0, {}}},
        {"\\Delta", {CommandType::SYMBOL, 0, {}}},
        {"\\Theta", {CommandType::SYMBOL, 0, {}}},
        {"\\Lambda", {CommandType::SYMBOL, 0, {}}},
        {"\\Xi", {CommandType::SYMBOL, 0, {}}},
        {"\\Pi", {CommandType::SYMBOL, 0, {}}},
        {"\\Sigma", {CommandType::SYMBOL, 0, {}}},
        {"\\Upsilon", {CommandType::SYMBOL, 0, {}}},
        {"\\Phi", {CommandType::SYMBOL, 0, {}}},
        {"\\Psi", {CommandType::SYMBOL, 0, {}}},
        {"\\Omega", {CommandType::SYMBOL, 0, {}}},

        // ======================
        // -- TRIG
        // ======================

        {"\\sin", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\cos", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\tan", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\csc", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\sec", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\cot", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\sinh", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\cosh", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\tanh", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\arcsin", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\arccos", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\arctan", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},

        // ======================
        // -- CALC / OPERATORS
        // ======================

        {"\\lim", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\sup", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\inf", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\max", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\min", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\log", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\ln", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\exp", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\det", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\dim", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\ker", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},
        {"\\gcd", {CommandType::UNARY, 4, {ArgType::REQUIRED}}},

        // ======================
        // -- INTERGRALS / SUMS / PRODUCTS
        // ======================

        {"\\int", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\iint", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\iiint", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\oint", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\sum", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\prod", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\bigcup", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},
        {"\\bigcap", {CommandType::UNARY, 2, {ArgType::REQUIRED}}},

        // ======================
        // -- FRACTIONS / ROOTS / MODIFIERS
        // ======================

        {"\\frac", {CommandType::BINARY, 1, {ArgType::REQUIRED, ArgType::REQUIRED}}},
        {"\\binom", {CommandType::BINARY, 1, {ArgType::REQUIRED, ArgType::REQUIRED}}},
        {"\\choose", {CommandType::BINARY, 1, {ArgType::REQUIRED, ArgType::REQUIRED}}},
        {"\\sqrt", {CommandType::UNARY, 3, {ArgType::OPTIONAL, ArgType::REQUIRED}}},
        {"\\bar", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\hat", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\tilde", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\dot", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\ddot", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\vec", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\overline", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\underline", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\overbrace", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\underbrace", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},

        // ======================
        // -- RELATIONS / LOGIC
        // ======================

        {"\\leq", {CommandType::SYMBOL, 0, {}}},
        {"\\geq", {CommandType::SYMBOL, 0, {}}},
        {"\\neq", {CommandType::SYMBOL, 0, {}}},
        {"\\equiv", {CommandType::SYMBOL, 0, {}}},
        {"\\approx", {CommandType::SYMBOL, 0, {}}},
        {"\\sim", {CommandType::SYMBOL, 0, {}}},
        {"\\cong", {CommandType::SYMBOL, 0, {}}},
        {"\\propto", {CommandType::SYMBOL, 0, {}}},
        {"\\prec", {CommandType::SYMBOL, 0, {}}},
        {"\\succ", {CommandType::SYMBOL, 0, {}}},
        {"\\subset", {CommandType::SYMBOL, 0, {}}},
        {"\\subseteq", {CommandType::SYMBOL, 0, {}}},
        {"\\supset", {CommandType::SYMBOL, 0, {}}},
        {"\\supseteq", {CommandType::SYMBOL, 0, {}}},
        {"\\in", {CommandType::SYMBOL, 0, {}}},
        {"\\notin", {CommandType::SYMBOL, 0, {}}},
        {"\\forall", {CommandType::SYMBOL, 0, {}}},
        {"\\exists", {CommandType::SYMBOL, 0, {}}},
        {"\\neg", {CommandType::SYMBOL, 0, {}}},
        {"\\land", {CommandType::SYMBOL, 0, {}}},
        {"\\lor", {CommandType::SYMBOL, 0, {}}},
        {"\\implies", {CommandType::SYMBOL, 0, {}}},
        {"\\iff", {CommandType::SYMBOL, 0, {}}},

        // ======================
        // -- ARROWS
        // ======================

        {"\\leftarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\rightarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\leftrightarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\Leftarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\Rightarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\Leftrightarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\uparrow", {CommandType::SYMBOL, 0, {}}},
        {"\\downarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\Uparrow", {CommandType::SYMBOL, 0, {}}},
        {"\\Downarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\hookleftarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\hookrightarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\mapsto", {CommandType::SYMBOL, 0, {}}},
        {"\\longleftarrow", {CommandType::SYMBOL, 0, {}}},
        {"\\longrightarrow", {CommandType::SYMBOL, 0, {}}},

        // ======================
        // -- DELIMS / GEOMETRY
        // ======================

        {"\\langle", {CommandType::SYMBOL, 0, {}}},
        {"\\rangle", {CommandType::SYMBOL, 0, {}}},
        {"\\lfloor", {CommandType::SYMBOL, 0, {}}},
        {"\\rfloor", {CommandType::SYMBOL, 0, {}}},
        {"\\lceil", {CommandType::SYMBOL, 0, {}}},
        {"\\rceil", {CommandType::SYMBOL, 0, {}}},
        {"\\angle", {CommandType::SYMBOL, 0, {}}},
        {"\\measuredangle", {CommandType::SYMBOL, 0, {}}},
        {"\\triangle", {CommandType::SYMBOL, 0, {}}},
        {"\\square", {CommandType::SYMBOL, 0, {}}},
        {"\\perp", {CommandType::SYMBOL, 0, {}}},
        {"\\parallel", {CommandType::SYMBOL, 0, {}}},

        // ======================
        // -- DOTS / MISC SYMBOLS
        // ======================

        {"\\cdot", {CommandType::SYMBOL, 0, {}}},
        {"\\times", {CommandType::SYMBOL, 0, {}}},
        {"\\pm", {CommandType::SYMBOL, 0, {}}},
        {"\\mp", {CommandType::SYMBOL, 0, {}}},
        {"\\div", {CommandType::SYMBOL, 0, {}}},
        {"\\star", {CommandType::SYMBOL, 0, {}}},
        {"\\bullet", {CommandType::SYMBOL, 0, {}}},
        {"\\dots", {CommandType::SYMBOL, 0, {}}},
        {"\\cdots", {CommandType::SYMBOL, 0, {}}},
        {"\\ldots", {CommandType::SYMBOL, 0, {}}},
        {"\\vdots", {CommandType::SYMBOL, 0, {}}},
        {"\\ddots", {CommandType::SYMBOL, 0, {}}},
        {"\\infty", {CommandType::SYMBOL, 0, {}}},
        {"\\emptyset", {CommandType::SYMBOL, 0, {}}},
        {"\\aleph", {CommandType::SYMBOL, 0, {}}},
        {"\\hbar", {CommandType::SYMBOL, 0, {}}},
        {"\\wp", {CommandType::SYMBOL, 0, {}}},

        // ======================
        // -- FONTS / TEXT
        // ======================

        {"\\text", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathrm", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathbb", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathbf", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathcal", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathfrak", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\mathit", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\operatorname", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},

        // ======================
        // -- ENV / STRUCTURES
        // ======================

        {"\\begin", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\end", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\cases", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\matrix", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\pmatrix", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\bmatrix", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\vmatrix", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
        {"\\Vmatrix", {CommandType::UNARY, 0, {ArgType::REQUIRED}}},
    };
}
