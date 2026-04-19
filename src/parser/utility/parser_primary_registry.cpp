#include <charconv>

#include "./parser_primary.hpp"
#include "../parser.hpp"
#include "../../lexer/token_info.hpp"
#include "../../ast/ast_node.hpp"
#include "../../ast/ast_arena.hpp"

// ======================
// -- INIT
// ======================

const std::unordered_map<TokenType, PrimaryParser::PrimaryHandler> PrimaryParser::PRIMARY_DISPATCH = {
	{TokenType::NUMBER, &PrimaryParser::parse_number},
	{TokenType::IDENTIFIER, &PrimaryParser::parse_identifier},
	{TokenType::ESCAPED_BRACE_OPEN, &PrimaryParser::parse_escaped_brace},
	{TokenType::BRACE_OPEN, &PrimaryParser::parse_group},
	{TokenType::PAREN_OPEN, &PrimaryParser::parse_group},
	{TokenType::BRACKET_OPEN, &PrimaryParser::parse_group},
	{TokenType::DISPLAY_MATH_OPEN, &PrimaryParser::parse_group},
	{TokenType::INLINE_MATH_OPEN, &PrimaryParser::parse_group},
	{TokenType::PUNCTUATION, &PrimaryParser::parse_symbol},
	{TokenType::SPACING, &PrimaryParser::parse_symbol},
	{TokenType::SYMBOL, &PrimaryParser::parse_symbol},
	{TokenType::ALIGNMENT, &PrimaryParser::parse_symbol},
	{TokenType::COMMAND, &PrimaryParser::parse_command},
	{TokenType::ENV_BEGIN, &PrimaryParser::parse_environment},
	{TokenType::LEFT_WRAP, &PrimaryParser::parse_left_right}};

const std::unordered_map<TokenType, TokenType> PrimaryParser::GROUP_CLOSERS = {
	{TokenType::BRACE_OPEN, TokenType::BRACE_CLOSE},
	{TokenType::PAREN_OPEN, TokenType::PAREN_CLOSE},
	{TokenType::BRACKET_OPEN, TokenType::BRACKET_CLOSE},
	{TokenType::DISPLAY_MATH_OPEN, TokenType::DISPLAY_MATH_CLOSE},
	{TokenType::INLINE_MATH_OPEN, TokenType::INLINE_MATH_CLOSE}};

// ======================
// -- PRIMARY DISPATCH
// ======================

/// @brief Parse a primary expression
/// @return AST node for primary
ASTNode *PrimaryParser::parse()
{
	auto it = PRIMARY_DISPATCH.find(_tok.Type);

	if (it != PRIMARY_DISPATCH.end())
		return (this->*it->second)();

	throw ParseError(
			"Unexpected token in primary: " + _parser.token_repr(_tok) + " @" +
			std::to_string(_tok.line) + ':' + std::to_string(_tok.column),
			_tok.line, _tok.column);
}

// ======================
// -- PRIMARY IMPL.
// ======================

/// @brief Parse a number literal
/// @return NumberNode
ASTNode *PrimaryParser::parse_number()
{
	Token tok = _parser.consume();
	double val = 0.0;

	auto [ptr, ec] = std::from_chars(tok.Value.data(), tok.Value.data() + tok.Value.size(), val);

	if (ec != std::errc{})
	{
		throw ParseError(
				"Invalid number @" + std::to_string(tok.line) + ':' + std::to_string(tok.column),
				tok.line, tok.column);
	}

	return make_node<NumberNode>(_parser._arena, val, tok.line, tok.column);
}

/// @brief Parse an identifier
/// @return VariableNode
ASTNode *PrimaryParser::parse_identifier()
{
	Token tok = _parser.consume();
	return make_node<VariableNode>(_parser._arena, tok.Value, tok.line, tok.column);
}

/// @brief Parse an escaped brace group
/// @return GroupNode
ASTNode *PrimaryParser::parse_escaped_brace()
{
	Token tok = _parser.consume();
	auto expr = _parser.parse_expression();
	_parser.expect(TokenType::ESCAPED_BRACE_CLOSE);
	return make_node<GroupNode>(_parser._arena, expr, tok.line, tok.column);
}

/// @brief Parse a grouped expression with a close token from GROUP_CLOSERS
/// @return GroupNode
ASTNode *PrimaryParser::parse_group()
{
	Token tok = _parser.consume();
	auto expr = _parser.parse_assignment();
	_parser.expect(GROUP_CLOSERS.at(tok.Type));
	return make_node<GroupNode>(_parser._arena, expr, tok.line, tok.column);
}

/// @brief Parse a symbol token (punctuation, spacing, symbol, alignment)
/// @return SymbolNode
ASTNode *PrimaryParser::parse_symbol()
{
	Token tok = _parser.consume();
	return make_node<SymbolNode>(_parser._arena, tok.Value, tok.line, tok.column);
}

/// @brief Delegate to Parser::parse_command
/// @return AST node for command
ASTNode *PrimaryParser::parse_command()
{
	return _parser.parse_command();
}

/// @brief Delegate to Parser::parse_environment
/// @return AST node for environment
ASTNode *PrimaryParser::parse_environment()
{
	return _parser.parse_environment();
}

/// @brief Delegate to Parser::parse_left_right
/// @return AST node for left-right
ASTNode *PrimaryParser::parse_left_right()
{
	return _parser.parse_left_right();
}
