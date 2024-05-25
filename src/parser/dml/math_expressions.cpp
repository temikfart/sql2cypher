#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

/**
 * @code
 * Parsing expression grammar (PEG):
 *   MathExpr ::= Sum
 *   Sum      ::= Product (('+' / '-') Product)?
 *   Product  ::= Power (('*' / '/') Power)?
 *   Power    ::= Value ('^' Power)?
 *   Value    ::= [0-9]+ / '(' MathExpr ')'
 * @endcode
 */
NodePtr<INode> Parser::ParseMathExpression() {
  NodePtr<INode> math_expression = ParseMathSum();
  return math_expression;
}
NodePtr<INode> Parser::ParseMathSum() {
  NodePtr<INode> lhs_product = ParseMathProduct();

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    NodePtr<INode> operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "+" || operator_str == "-") {
      NextToken();
      ASTUtils::Link(operator_node, lhs_product);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'{}\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> rhs_product = ParseMathProduct();
      ASTUtils::Link(operator_node, rhs_product);
      return operator_node;
    }
  }

  return lhs_product;
}
NodePtr<INode> Parser::ParseMathProduct() {
  NodePtr<INode> lhs_power = ParseMathPower();

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    NodePtr<INode> operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "*" || operator_str == "/") {
      NextToken();
      ASTUtils::Link(operator_node, lhs_power);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> rhs_power = ParseMathPower();
      ASTUtils::Link(operator_node, rhs_power);
      return operator_node;
    }
  }

  return lhs_power;
}
NodePtr<INode> Parser::ParseMathPower() {
  NodePtr<INode> value = ParseMathValue();

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    NodePtr<INode> operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "^") {
      NextToken();
      ASTUtils::Link(operator_node, value);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'{}\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> power = ParseMathPower();
      ASTUtils::Link(operator_node, power);
      return operator_node;
    }
  }

  return value;
}
NodePtr<INode> Parser::ParseMathValue() {
  auto peeked_token = PeekToken();

  if (NodeDataTypeClassifier::IsNumber(peeked_token)) {
    NodePtr<INode> number = NextToken();
    return number;
  }

  std::string msg_prefix = "Incorrect math expression at line ";

  if (NodeDataClassifier::IsOpeningRoundBracket(peeked_token)) {
    auto next_token = NextToken();

    ValidateHasTokens(format("{} {}: incorrect parenthesis sequence \'(\'",
                             msg_prefix, next_token->line));
    NodePtr<INode> math_expression = ParseMathExpression();

    ValidateHasTokens(format("{} {}: missing closing parenthesis",
                             msg_prefix, math_expression->line));
    next_token = NextToken();
    ValidateIsClosingRoundBracket(next_token);

    return math_expression;
  }

  throw parsing_error(format("{} {}: expected number or math expression in parentheses",
                             msg_prefix, peeked_token->line));
}

} // scc::parser
