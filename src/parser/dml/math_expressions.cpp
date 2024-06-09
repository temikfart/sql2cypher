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
  auto math_expression = ASTUtils::CreateServiceNode(StmtType::kMathExpression);

  auto sum = ParseMathSum();
  ASTUtils::Link(math_expression, sum);

  return math_expression;
}
NodePtr<INode> Parser::ParseMathSum() {
  auto sum = ASTUtils::CreateServiceNode(StmtType::kSum);

  NodePtr<INode> lhs_product = ParseMathProduct();
  ASTUtils::Link(sum, lhs_product);

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    auto operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "+" || operator_str == "-") {
      NextToken();
      ASTUtils::Link(sum, operator_node);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'{}\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> rhs_product = ParseMathProduct();
      ASTUtils::Link(sum, rhs_product);
    }
  }

  return sum;
}
NodePtr<INode> Parser::ParseMathProduct() {
  auto product = ASTUtils::CreateServiceNode(StmtType::kProduct);

  NodePtr<INode> lhs_power = ParseMathPower();
  ASTUtils::Link(product, lhs_power);

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    NodePtr<INode> operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "*" || operator_str == "/") {
      NextToken();
      ASTUtils::Link(product, operator_node);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> rhs_power = ParseMathPower();
      ASTUtils::Link(product, rhs_power);
    }
  }

  return product;
}
NodePtr<INode> Parser::ParseMathPower() {
  auto power = ASTUtils::CreateServiceNode(StmtType::kPower);

  NodePtr<INode> value = ParseMathValue();
  ASTUtils::Link(power, value);

  if (HasTokens() && NodeDataTypeClassifier::IsOperator(PeekToken())) {
    NodePtr<INode> operator_node = PeekToken();
    std::string operator_str = ASTUtils::CastToNodeType<StringNode>(operator_node)->data;
    if (operator_str == "^") {
      NextToken();
      ASTUtils::Link(power, operator_node);

      ValidateHasTokens(format("Incorrect math expression at line {}"
                               ": missing second operand for \'{}\' binary operator",
                               operator_node->line, operator_str));
      NodePtr<INode> next_power = ParseMathPower();
      ASTUtils::Link(power, next_power);
    }
  }

  return power;
}
NodePtr<INode> Parser::ParseMathValue() {
  auto value = ASTUtils::CreateServiceNode(StmtType::kValue);

  auto peeked_token = PeekToken();
  if (NodeDataTypeClassifier::IsNumber(peeked_token)) {
    NodePtr<INode> number = NextToken();
    ASTUtils::Link(value, number);
    return value;
  }

  std::string msg_prefix = "Incorrect math expression at line ";
  if (NodeDataClassifier::IsOpeningRoundBracket(peeked_token)) {
    auto next_token = NextToken();
    ValidateHasTokens(format("{} {}: incorrect parenthesis sequence \'(\'",
                             msg_prefix, next_token->line));
    NodePtr<INode> math_expression = ParseMathExpression();
    ASTUtils::Link(value, math_expression);

    ValidateHasTokens(format("{} {}: missing closing parenthesis",
                             msg_prefix, math_expression->line));
    next_token = NextToken();
    ValidateIsClosingRoundBracket(next_token);

    return value;
  }

  throw parsing_error(format("{} {}: expected number or math expression in parentheses",
                             msg_prefix, peeked_token->line));
}

} // scc::parser
