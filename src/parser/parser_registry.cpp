#include <string>
#include <array>

#include "./parser.hpp"
#include "../lexer/token_info.hpp"
#include "../ast/ast_node.hpp"
#include "../ast/ast_arena.hpp"
#include "./utility/parser_primary.hpp"

// ======================
// -- INIT
// ======================

namespace
{
	struct ImplicitMulTable
	{
		std::array<bool, 256> data{};

		constexpr ImplicitMulTable()
		{
			data[static_cast<size_t>(TokenType::NUMBER)] = true;
			data[static_cast<size_t>(TokenType::IDENTIFIER)] = true;
			data[static_cast<size_t>(TokenType::COMMAND)] = true;
			data[static_cast<size_t>(TokenType::PAREN_OPEN)] = true;
			data[static_cast<size_t>(TokenType::BRACE_OPEN)] = true;
			data[static_cast<size_t>(TokenType::ESCAPED_BRACE_OPEN)] = true;
			data[static_cast<size_t>(TokenType::SPACING)] = true;
		}
	};

	struct ExpressionOpTable
	{
		std::array<char, 256> data{};

		constexpr ExpressionOpTable()
		{
			for (int i = 0; i < 256; i++)
				data[i] = '\0';

			data[static_cast<size_t>(TokenType::PLUS)] = '+';
			data[static_cast<size_t>(TokenType::MINUS)] = '-';
			data[static_cast<size_t>(TokenType::PLUS_MINUS)] = 'P';
			data[static_cast<size_t>(TokenType::MINUS_PLUS)] = 'M';
		}
	};

	struct RelationalOpTable
	{
		std::array<char, 256> data{};

		constexpr RelationalOpTable()
		{
			for (int i = 0; i < 256; i++)
				data[i] = '\0';

			data[static_cast<size_t>(TokenType::LESS)] = '<';
			data[static_cast<size_t>(TokenType::GREATER)] = '>';
			data[static_cast<size_t>(TokenType::LESS_EQUAL)] = 'L';
			data[static_cast<size_t>(TokenType::GREATER_EQUAL)] = 'G';
		}
	};

	static constexpr ImplicitMulTable MUL_LOOKUP;
	static constexpr ExpressionOpTable EXPR_OP_LOOKUP;
	static constexpr RelationalOpTable REL_OP_LOOKUP;
}

const std::unordered_map<TokenType, Parser::PostfixHandler> Parser::POSTFIX_DISPATCH = {
	{TokenType::PAREN_OPEN, &Parser::try_function_call},
	{TokenType::BRACE_OPEN, &Parser::try_braced_call},
	{TokenType::ESCAPED_BRACE_OPEN, &Parser::try_braced_call},
	{TokenType::SUBSCRIPT, &Parser::parse_subsup},
	{TokenType::SUPERSCRIPT, &Parser::parse_subsup},
	{TokenType::FACTORIAL, &Parser::parse_factorial}};

// ======================
// -- HELPER IMPL.
// ======================

/// @brief Check if at end of token stream
/// @return True if at end
bool Parser::is_at_end() const
{
	return _tokens[_position].Type == TokenType::END_OF_FILE;
}

/// @brief Get the current token
/// @return Current token before advancing
Token Parser::current() const
{
	return _tokens[_position];
}

/// @brief Peek at next
/// @return Current token
Token Parser::peek_next() const
{
	if (_position + 1 >= _tokens.size())
		return _tokens.back();

	return _tokens[_position + 1];
}

/// @brief Consume the current token
/// @return Token
Token Parser::consume()
{
	return _tokens[_position++];
}

/// @brief Check if current token matches given type
/// @param type: Token type to match
/// @return True if current token matches type
bool Parser::match(TokenType type)
{
	if (is_at_end()) [[unlikely]]
		return false;

	return current().Type == type;
}

/// @brief Expect a specific token type or throw error
/// @param type: Expected token type
/// @param msg: Optional message
/// @return The matched token
Token Parser::expect(TokenType type, const std::string &msg)
{
	if (_tokens[_position].Type != type)
	{
		std::string error_msg = msg.empty()
			? "Expected Token Type: (" + std::to_string(static_cast<int>(type)) + ")"
			: std::string(msg);

		Token err_tok = current();

		throw ParseError(error_msg + " but found " + token_repr(err_tok),
				err_tok.line, err_tok.column);
	}

	return consume();
}

/// @brief Get string representation of a token
/// @param token: The token to represent
/// @return std::string
std::string Parser::token_repr(const Token &token) const
{
	return "'" + std::string(token.Value) + "' (" + std::to_string(static_cast<int>(token.Type)) + ")";
}

// ======================
// -- PARSING IMPL.
// ======================

/// @brief Parse tokens into an AST
/// @return Root node of the AST
/// @throws ParseError if parsing fails
ASTNode *Parser::parse()
{
	auto node = parse_root();

	if (!is_at_end())
	{
		Token current_token = current();

		throw ParseError(
				"Unexpected token " + token_repr(current_token) +
				" after complete expression @" +
				std::to_string(current_token.line) + ':' +
				std::to_string(current_token.column),
				current_token.line,
				current_token.column);
	}

	return node;
}

/// @brief Parse root of the AST
/// @return Root node of the AST
ASTNode *Parser::parse_root()
{
	std::vector<ASTNode *> lines;
	lines.reserve(8);

	while (!is_at_end())
	{
		if (match(TokenType::NEWLINE) || match(TokenType::SPACING))
		{
			consume();
			continue;
		}

		lines.push_back(parse_statement());
	}

	if (lines.empty())
		return nullptr;

	return lines.size() > 1
		? make_node<SequenceNode>(_arena, lines, lines[0]->line, lines[0]->column)
		: lines[0];
}

/// @brief Parse a environment
/// @return Environment AST node
ASTNode *Parser::parse_environment()
{
	Token current_token = consume();

	expect(TokenType::BRACE_OPEN);
	std::string name = std::string(expect(TokenType::IDENTIFIER).Value);
	expect(TokenType::BRACE_CLOSE);

	if (match(TokenType::BRACE_OPEN))
	{
		consume();

		while (!is_at_end() && !match(TokenType::BRACE_CLOSE))
			consume();

		expect(TokenType::BRACE_CLOSE);
	}

	std::vector<std::vector<ASTNode *>> body;
	std::vector<ASTNode *> current_line;

	while (!is_at_end())
	{
		if (match(TokenType::ENV_END))
		{
			consume();

			expect(TokenType::BRACE_OPEN);

			if (expect(TokenType::IDENTIFIER).Value == name)
			{
				expect(TokenType::BRACE_CLOSE);
				if (!current_line.empty())
					body.push_back(std::move(current_line));
				break;
			}

			throw ParseError("Mismatched environment closure", current_token.line, current_token.column);
		}

		current_line.push_back(parse_assignment());

		if (match(TokenType::ALIGNMENT))
		{
			consume();
		}
		else if (match(TokenType::NEWLINE))
		{
			consume();
			body.push_back(std::move(current_line));
			current_line.clear();
		}
	}

	return make_node<EnvironmentNode>(_arena, name, body, current_token.line, current_token.column);
}

/// @brief Parse a left-right construct
/// @return LeftRight AST node
ASTNode *Parser::parse_left_right()
{
	Token current_token = current();

	int start_line = current_token.line;
	int start_col = current_token.column;

	consume();

	Token left_delim = consume();

	ASTNode *inner = parse_assignment();

	if (!match(TokenType::RIGHT_WRAP))
	{
		Token err_token = current();

		throw ParseError(
				"Missing \\right to match \\left @" +
				std::to_string(start_line) + ":" + std::to_string(start_col),
				err_token.line,
				err_token.column);
	}

	consume();

	Token right_delim = consume();

	return make_node<LeftRightNode>(
			_arena,
			std::string(left_delim.Value),
			std::string(right_delim.Value),
			inner,
			start_line,
			start_col);
}

/// @brief Parse a statement
/// @return Statement AST node
ASTNode *Parser::parse_statement()
{
	return parse_assignment();
}

// ======================
// -- PRECEDENCE IMPL.
// ======================

/// @brief Parse a assignment statement
/// @return AST node for assignment
ASTNode *Parser::parse_assignment()
{
	ASTNode *left;

	if (match(TokenType::EQUAL) || match(TokenType::ALIGNMENT))
	{
		left = make_node<SymbolNode>(_arena, "", current().line, current().column);
	}
	else
	{
		left = parse_relational();
	}

	if (match(TokenType::EQUAL) || match(TokenType::ALIGNMENT))
	{
		Token op = consume();
		auto right = parse_assignment();

		if (op.Type == TokenType::EQUAL)
			return make_node<AssignNode>(_arena, left, right, op.line, op.column);
		else
			return make_node<BinaryOpNode>(_arena, '&', left, right, op.line, op.column);
	}

	return left;
}

/// @brief Parse relational expressions
/// @return AST node for relational expression
ASTNode *Parser::parse_relational()
{
	auto left = parse_expression();

	TokenType current_token_type = current().Type;

	while (current_token_type == TokenType::LESS || current_token_type == TokenType::GREATER ||
			current_token_type == TokenType::LESS_EQUAL || current_token_type == TokenType::GREATER_EQUAL)
	{
		Token op = consume();
		auto right = parse_expression();

		char oper = REL_OP_LOOKUP.data[static_cast<size_t>(op.Type)];

		left = make_node<BinaryOpNode>(_arena, oper, left, right, op.line, op.column);

		current_token_type = current().Type;
	}

	return left;
}

/// @brief Parse a complete expression
/// @return AST node for expression
ASTNode *Parser::parse_expression()
{
	auto left = parse_term();

	while (match(TokenType::PLUS) || match(TokenType::MINUS) || match(TokenType::PLUS_MINUS) || match(TokenType::MINUS_PLUS))
	{
		Token op = consume();
		auto right = parse_term();

		char oper = EXPR_OP_LOOKUP.data[static_cast<size_t>(op.Type)];

		left = make_node<BinaryOpNode>(_arena, oper, left, right, op.line, op.column);
	}

	return left;
}

/// @brief Parse a term
/// @return AST node for term
ASTNode *Parser::parse_term()
{
	auto left = parse_power();

	while (match(TokenType::STAR) || match(TokenType::SLASH))
	{
		Token op = consume();
		auto right = parse_power();
		char oper = (op.Type == TokenType::STAR) ? '*' : '/';

		left = make_node<BinaryOpNode>(_arena, oper, left, right, op.line, op.column);
	}

	return left;
}

/// @brief Parse a power / exponentiation
/// @return AST node for power
ASTNode *Parser::parse_power()
{
	auto base = parse_prefix();

	if (match(TokenType::CARET) || match(TokenType::SUPERSCRIPT))
	{
		Token op = consume();
		auto exponent = parse_power();

		return make_node<BinaryOpNode>(_arena, '^', base, exponent, op.line, op.column);
	}

	return base;
}

/// @brief Parse a factor
/// @return AST node for factor
ASTNode *Parser::parse_prefix()
{
	if (match(TokenType::MINUS) || match(TokenType::PLUS))
	{
		Token op = consume();
		auto expr = parse_prefix();
		char oper = (op.Type == TokenType::MINUS) ? '-' : '+';

		return make_node<UnaryOpNode>(_arena, oper, expr, op.line, op.column);
	}

	return parse_postfix();
}

/// @brief Parse a postfix expression
/// @return AST node for postfix
ASTNode *Parser::parse_postfix()
{
	auto expr = parse_primary();

	while (!is_at_end())
	{
		auto it = POSTFIX_DISPATCH.find(current().Type);

		if (it == POSTFIX_DISPATCH.end())
			break;

		PostfixHandler handler = it->second;
		expr = (this->*handler)(expr);
	}

	return try_implicit_mul(expr);
}

/// @brief Parse a primary expression
/// @return AST node for primary
ASTNode *Parser::parse_primary()
{
	return PrimaryParser(*this, current()).parse();
}

// ======================
// -- LATEX IMPL.
// ======================

/// @brief Parse a LaTeX command
/// @return AST node for command
ASTNode *Parser::parse_command()
{
	Token cmd_token = consume();
	const CommandInfo *info = cmd_token.Info;

	if (!info)
		return make_node<SymbolNode>(_arena, cmd_token.Value, cmd_token.line, cmd_token.column);

	std::vector<ASTNode *> args;
	args.reserve(info->mandatory_args + info->optional_args);

	for (int i = 0; i < info->optional_args; ++i)
	{
		if (match(TokenType::BRACKET_OPEN))
		{
			consume();

			args.push_back(parse_assignment());
			expect(TokenType::BRACKET_CLOSE, "Expected ']' after optional argument");
		}
		else
		{
			args.push_back(nullptr);
		}
	}

	bool requires_braces = (info->mandatory_args > 1);

	for (int i = 0; i < info->mandatory_args; ++i)
	{
		if (match(TokenType::BRACE_OPEN))
		{
			consume();
			args.push_back(parse_assignment());
			expect(TokenType::BRACE_CLOSE, "Expected '}' after mandatory argument");
		}
		else if (requires_braces)
		{
			throw ParseError(
					"Command '" + std::string(cmd_token.Value) + "' requires braced arguments",
					cmd_token.line, cmd_token.column);
		}
		else
		{
			args.push_back(parse_primary());
		}
	}

	return make_node<CommandNode>(_arena, cmd_token.Value, args, info, cmd_token.line, cmd_token.column);
}

/// @brief Parse subscripts and superscripts
/// @param base: Base AST node
/// @return AST node for subscript / superscript
ASTNode *Parser::parse_subsup(ASTNode *base)
{
	ASTNode *sub = nullptr;
	ASTNode *sup = nullptr;

	while (match(TokenType::SUBSCRIPT) || match(TokenType::SUPERSCRIPT))
	{
		bool is_super = match(TokenType::SUPERSCRIPT);
		consume();

		if ((is_super && sup) || (!is_super && sub))
			throw ParseError("Multiple scripts of the same type detected", current().line, current().column);

		ASTNode *script;

		if (match(TokenType::BRACE_OPEN))
		{
			consume();

			script = parse_assignment();
			expect(TokenType::BRACE_CLOSE);
		}
		else
		{
			script = parse_prefix();
		}

		if (is_super)
			sup = script;
		else
			sub = script;
	}

	return make_node<ScriptNode>(_arena, base, sub, sup, base->line, base->column);
}

/// @brief Parse a factorial operator
/// @param left: The operand to the left of the '!'
/// @return A UnaryOpNode representing the factorial
ASTNode *Parser::parse_factorial(ASTNode *left)
{
	Token op = consume();
	return make_node<UnaryOpNode>(_arena, '!', left, op.line, op.column);
}

// ======================
// -- MISC IMPL.
// ======================

/// @brief Try to parse implicit multiplication
/// @param left: Left AST node
/// @return AST node for implicit multiplication or left node if no implicit multiplication
ASTNode *Parser::try_implicit_mul(ASTNode *left)
{
	while (!is_at_end())
	{
		if (!MUL_LOOKUP.data[static_cast<size_t>(current().Type)])
			break;

		size_t last_pos = _position;
		auto right = parse_prefix();

		if (_position == last_pos)
			break;

		left = make_node<BinaryOpNode>(_arena, '*', left, right, left->line, left->column);
	}

	return left;
}

/// @brief Try a function call
/// @param func The function
/// @return AST node for function call or no function call
ASTNode *Parser::try_function_call(ASTNode *func)
{
	if (!match(TokenType::PAREN_OPEN))
		return func;

	Token open_paren = consume();

	std::vector<ASTNode *> args;

	if (!match(TokenType::PAREN_CLOSE))
	{
		args.reserve(4);
		args.push_back(parse_assignment());

		while (match(TokenType::PUNCTUATION) && current().Value == ",")
		{
			consume();
			args.push_back(parse_assignment());
		}
	}

	expect(TokenType::PAREN_CLOSE, "Expected ')' after function arguments");

	return make_node<FunctionCallNode>(_arena, func, args, open_paren.line, open_paren.column);
}

/// @brief Try to parse arguments in curly braces
/// @param base The preceding node (the "function" or "operator")
/// @return AST node representing the applying of the braces to the base
ASTNode *Parser::try_braced_call(ASTNode *base)
{
	bool is_escaped = (current().Type == TokenType::ESCAPED_BRACE_OPEN);

	Token opening = consume();

	auto arg = parse_assignment();

	if (is_escaped)
	{
		expect(TokenType::ESCAPED_BRACE_CLOSE, "Expected '\\}' after escaped group");
	}
	else
	{
		expect(TokenType::BRACE_CLOSE, "Expected '}' after group");
	}

	std::vector<ASTNode *> args;
	args.push_back(arg);

	return make_node<FunctionCallNode>(_arena, base, args, opening.line, opening.column);
}
