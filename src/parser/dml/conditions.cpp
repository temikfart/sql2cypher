#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::ParseCondition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kCondition);
  NodePtr<INode> or_condition = ParseORCondition();
  ASTUtils::Link(service_node, or_condition);
  return service_node;
}
NodePtr<INode> Parser::ParseORCondition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kORCondition);
  NodePtr<INode> and_condition = ParseANDCondition();
  ASTUtils::Link(service_node, and_condition);

  auto peeked_token = PeekToken();
  if (HasTokens() && NodeDataTypeClassifier::IsWord(peeked_token)) {
    std::string checking_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
    if (DetermineIsOROperator(checking_word)) {
      NextToken();
      ValidateHasTokens("Incorrect \'OR\' condition: expected \'AND\' condition"
                        "as second operand at line " + std::to_string(peeked_token->line));
      NodePtr<INode> next_and_conditions = ParseANDCondition();
      ASTUtils::Link(service_node, next_and_conditions);
    }
  }

  return service_node;
}
NodePtr<INode> Parser::ParseANDCondition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kANDCondition);
  NodePtr<INode> not_condition = ParseNOTCondition();
  ASTUtils::Link(service_node, not_condition);

  auto peeked_token = PeekToken();
  if (HasTokens() && NodeDataTypeClassifier::IsWord(peeked_token)) {
    std::string checking_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
    if (DetermineIsANDOperator(checking_word)) {
      NextToken();
      ValidateHasTokens("Incorrect \'AND\' condition: expected \'NOT\' condition"
                        "as second operand at line " + std::to_string(peeked_token->line));
      NodePtr<INode> next_not_conditions = ParseNOTCondition();
      ASTUtils::Link(service_node, next_not_conditions);
    }
  }

  return service_node;
}
NodePtr<INode> Parser::ParseNOTCondition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kNOTCondition);

  auto peeked_token = PeekToken();
  if (NodeDataTypeClassifier::IsWord(peeked_token)) {
    std::string checking_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
    if (DetermineIsNOTOperator(checking_word)) {
      NodePtr<INode> not_operator = NextToken();
      ASTUtils::Link(service_node, not_operator);
    }
  }

  ValidateHasTokens("Incorrect \'NOT\' condition: expected predicate at line "
                        + std::to_string(peeked_token->line));
  NodePtr<INode> predicate = ParsePredicate();
  ASTUtils::Link(service_node, predicate);

  return service_node;
}
NodePtr<INode> Parser::ParsePredicate() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kPredicate);
  NodePtr<INode> first_operand = ParseExpression();
  ASTUtils::Link(service_node, first_operand);

  auto peeked_token = PeekToken();
  ValidateHasTokens("Incorrect predicate at line " + std::to_string(peeked_token->line)
                        + ": expected binary operator");
  ValidateIsBinaryOperator(peeked_token);
  NodePtr<INode> binary_operator = NextToken();
  ASTUtils::Link(service_node, binary_operator);

  ValidateHasTokens("Incorrect predicate at line " + std::to_string(peeked_token->line)
                        + ": expected second operand for binary operator");
  NodePtr<INode> second_operand = ParseExpression();
  ASTUtils::Link(service_node, second_operand);

  return service_node;
}
NodePtr<INode> Parser::ParseExpression() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kExpression);

  auto peeked_token = PeekToken();

  // Is it [table_name.] column ?
  if (NodeDataTypeClassifier::IsWord(peeked_token)) {
    NodePtr<INode> name = ParseName();
    ASTUtils::Link(service_node, name);

    return service_node;
  }

  // Is it unary operator ?
  if (NodeDataClassifier::IsUnaryOperator(peeked_token)) {
    NodePtr<INode> unary_operator = NextToken();
    ASTUtils::Link(service_node, unary_operator);

    ValidateHasTokens("Incorrect unary operator at line " + std::to_string(peeked_token->line)
                          + ": expected operand");
    NodePtr<INode> expression = ParseExpression();
    ASTUtils::Link(service_node, expression);

    return service_node;
  }

  // Is it (expression) ?
  if (NodeDataClassifier::IsOpeningRoundBracket(peeked_token)) {
    NextToken();

    ValidateHasTokens("Incorrect expression at line " + std::to_string(peeked_token->line)
                          + ": incorrect parenthesis sequence \'(\'");
    service_node = ParseExpression();

    ValidateHasTokens("Incorrect expression at line " + std::to_string(peeked_token->line)
                          + ": missing closing parenthesis");
    ValidateIsClosingRoundBracket(NextToken());

    return service_node;
  }

  // Is it string ?
  if (NodeDataClassifier::IsQuote(PeekToken())) {
    NextToken();

    ValidateHasTokens("Incorrect expression at line " + std::to_string(peeked_token->line)
                          + ": expected string literal or closing quote");
    NodePtr<INode> string = ParseString();
    ASTUtils::Link(service_node, string);

    return service_node;
  }

  // It is Math expression
  NodePtr<INode> math_expression = ParseMathExpression();
  ASTUtils::Link(service_node, math_expression);

  return service_node;
}

} // scc::parser
