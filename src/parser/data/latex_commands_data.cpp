#include "./latex_commands.hpp"

// ======================
// -- INIT
// ======================

namespace LatexParser {
    const std::vector<CommandInfo> LATEX_COMMANDS = {
        // ======== GREEK LETTERS (Lowercase) ========

        { "\\alpha", CommandType::SYMBOL, 0, 0 },
        { "\\beta", CommandType::SYMBOL, 0, 0 },
        { "\\gamma", CommandType::SYMBOL, 0, 0 },
        { "\\delta", CommandType::SYMBOL, 0, 0 },
        { "\\epsilon", CommandType::SYMBOL, 0, 0 },
        { "\\zeta", CommandType::SYMBOL, 0, 0 },
        { "\\eta", CommandType::SYMBOL, 0, 0 },
        { "\\theta", CommandType::SYMBOL, 0, 0 },
        { "\\iota", CommandType::SYMBOL, 0, 0 },
        { "\\kappa", CommandType::SYMBOL, 0, 0 },
        { "\\lambda", CommandType::SYMBOL, 0, 0 },
        { "\\mu", CommandType::SYMBOL, 0, 0 },
        { "\\nu", CommandType::SYMBOL, 0, 0 },
        { "\\xi", CommandType::SYMBOL, 0, 0 },
        { "\\pi", CommandType::SYMBOL, 0, 0 },
        { "\\rho", CommandType::SYMBOL, 0, 0 },
        { "\\sigma", CommandType::SYMBOL, 0, 0 },
        { "\\tau", CommandType::SYMBOL, 0, 0 },
        { "\\upsilon", CommandType::SYMBOL, 0, 0 },
        { "\\phi", CommandType::SYMBOL, 0, 0 },
        { "\\chi", CommandType::SYMBOL, 0, 0 },
        { "\\psi", CommandType::SYMBOL, 0, 0 },
        { "\\omega", CommandType::SYMBOL, 0, 0 },

        
        // ======== GREEK LETTERS (Uppercase) ========

        { "\\Gamma", CommandType::SYMBOL, 0, 0 },
        { "\\Delta", CommandType::SYMBOL, 0, 0 },
        { "\\Theta", CommandType::SYMBOL, 0, 0 },
        { "\\Lambda", CommandType::SYMBOL, 0, 0 },
        { "\\Xi", CommandType::SYMBOL, 0, 0 },
        { "\\Pi", CommandType::SYMBOL, 0, 0 },
        { "\\Sigma", CommandType::SYMBOL, 0, 0 },
        { "\\Upsilon", CommandType::SYMBOL, 0, 0 },
        { "\\Phi", CommandType::SYMBOL, 0, 0 },
        { "\\Psi", CommandType::SYMBOL, 0, 0 },
        { "\\Omega", CommandType::SYMBOL, 0, 0 },
        
        
        // ======== BINARY OPERATORS  ========

        { "\\frac", CommandType::BINARY, 1, 2 },
        { "\\binom", CommandType::BINARY, 1, 2 },        

        // ========  ROOTS ========

        { "\\sqrt", CommandType::UNARY, 3, 1 },
        
        // ======== TRIG ========

        { "\\sin", CommandType::UNARY, 4, 1 },
        { "\\cos", CommandType::UNARY, 4, 1 },
        { "\\tan", CommandType::UNARY, 4, 1 },
        { "\\cot", CommandType::UNARY, 4, 1 },
        { "\\sec", CommandType::UNARY, 4, 1 },
        { "\\csc", CommandType::UNARY, 4, 1 },
        
        // ======== INVERSE TRIG ========

        { "\\arcsin", CommandType::UNARY, 4, 1 },
        { "\\arccos", CommandType::UNARY, 4, 1 },
        { "\\arctan", CommandType::UNARY, 4, 1 },

        // ========  HYPERBOLIC FUNCTIONS ========

        { "\\sinh", CommandType::UNARY, 4, 1 },
        { "\\cosh", CommandType::UNARY, 4, 1 },
        { "\\tanh", CommandType::UNARY, 4, 1 },

        // ======== LOG FUNCTIONS ========

        { "\\ln", CommandType::UNARY, 4, 1 },
        { "\\log", CommandType::UNARY, 4, 1 },
        { "\\exp", CommandType::UNARY, 4, 1 },
        
        // ======== CALCULUS ========

        { "\\lim", CommandType::UNARY, 4, 1 },
        { "\\int", CommandType::UNARY, 2, 1 },
        { "\\sum", CommandType::UNARY, 2, 1 },
        { "\\prod", CommandType::UNARY, 2, 1 },

        // ======== ARROWS ========

        { "\\rightarrow", CommandType::SYMBOL, 0, 0 },
        { "\\leftarrow", CommandType::SYMBOL, 0, 0 },
        { "\\Rightarrow", CommandType::SYMBOL, 0, 0 },
        { "\\Leftarrow", CommandType::SYMBOL, 0, 0 },
        { "\\leftrightarrow", CommandType::SYMBOL, 0, 0 },
        { "\\Leftrightarrow", CommandType::SYMBOL, 0, 0 },

        // ========  RELATIONS ========

        { "\\leq", CommandType::SYMBOL, 0, 0 },
        { "\\geq", CommandType::SYMBOL, 0, 0 },
        { "\\neq", CommandType::SYMBOL, 0, 0 },
        { "\\approx", CommandType::SYMBOL, 0, 0 },
        { "\\equiv", CommandType::SYMBOL, 0, 0 },
        { "\\sim", CommandType::SYMBOL, 0, 0 },
        { "\\propto", CommandType::SYMBOL, 0, 0 },

        // ======== SET OPERATORS ========

        { "\\in", CommandType::SYMBOL, 0, 0 },
        { "\\notin", CommandType::SYMBOL, 0, 0 },
        { "\\subset", CommandType::SYMBOL, 0, 0 },
        { "\\subseteq", CommandType::SYMBOL, 0, 0 },
        { "\\supset", CommandType::SYMBOL, 0, 0 },
        { "\\supseteq", CommandType::SYMBOL, 0, 0 },
        { "\\cup", CommandType::SYMBOL, 0, 0 },
        { "\\cap", CommandType::SYMBOL, 0, 0 },
        { "\\emptyset", CommandType::SYMBOL, 0, 0 },

        // ======== SPECIAL SYMBOLS ========

        { "\\infty", CommandType::SYMBOL, 0, 0 },
        { "\\partial", CommandType::SYMBOL, 0, 0 },
        { "\\nabla", CommandType::SYMBOL, 0, 0 },
        { "\\forall", CommandType::SYMBOL, 0, 0 },
        { "\\exists", CommandType::SYMBOL, 0, 0 },
        { "\\neg", CommandType::SYMBOL, 0, 0 },
        { "\\wedge", CommandType::SYMBOL, 0, 0 },
        { "\\vee", CommandType::SYMBOL, 0, 0 },
        { "\\pm", CommandType::SYMBOL, 0, 0 },
        { "\\mp", CommandType::SYMBOL, 0, 0 },
        { "\\times", CommandType::SYMBOL, 0, 0 },
        { "\\div", CommandType::SYMBOL, 0, 0 },
        { "\\cdot", CommandType::SYMBOL, 0, 0 },

        // ======== OTHER ========

        { "\\max", CommandType::UNARY, 4, 1 },
        { "\\min", CommandType::UNARY, 4, 1 },
        { "\\sup", CommandType::UNARY, 4, 1 },
        { "\\inf", CommandType::UNARY, 4, 1 },
        { "\\det", CommandType::UNARY, 4, 1 },
        { "\\dim", CommandType::UNARY, 4, 1 },
        { "\\ker", CommandType::UNARY, 4, 1 },
        { "\\gcd", CommandType::UNARY, 4, 1 },
    };
}
