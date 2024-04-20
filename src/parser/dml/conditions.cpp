#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::GetCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kCondition);
  NodePtr<INode> OR_condition = GetORCondition();
  ASTUtils::Link(node, OR_condition);

  return node;
}
NodePtr<INode> Parser::GetORCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kORCondition);
  NodePtr<INode> AND_condition = GetANDCondition();
  ASTUtils::Link(node, AND_condition);

  int line = PeekToken()->line;
  if (!tokens_.empty() && NodeDataTypeClassifier::IsWord(PeekToken())) {
    NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(PeekToken());
    if (tmp->data == "OR") {
      NextToken();

      if (tokens_.empty()) {
        LOGE << "invalid OR-condition: expected AND-condition "
                "after the \'OR\' logical operator in line " << line;
        end(EXIT_FAILURE);
      }
      NodePtr<INode> next_AND_conditions = GetANDCondition();
      ASTUtils::Link(node, next_AND_conditions);
    }
  }

  return node;
}
NodePtr<INode> Parser::GetANDCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kANDCondition);
  NodePtr<INode> NOT_condition = GetNOTCondition();
  ASTUtils::Link(node, NOT_condition);

  int line = PeekToken()->line;
  if (!tokens_.empty()) {
    if (NodeDataTypeClassifier::IsWord(PeekToken())) {
      NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(PeekToken());
      if (tmp->data == "AND") {
        NextToken();

        if (tokens_.empty()) {
          LOGE << "invalid AND-condition: expected NOT-condition "
                  "after the \\'AND\\' operator in line " << line;
          end(EXIT_FAILURE);
        }
        NodePtr<INode> next_NOT_conditions = GetNOTCondition();
        ASTUtils::Link(node, next_NOT_conditions);
      }
    }
  }

  return node;
}
NodePtr<INode> Parser::GetNOTCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kNOTCondition);

  // Get NOT if present
  int line = PeekToken()->line;
  if (NodeDataTypeClassifier::IsWord(PeekToken())) {
    NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(PeekToken());
    if (tmp->data == "NOT") {
      NodePtr<INode> NOT_operator = NextToken();

      ASTUtils::Link(node, NOT_operator);
    }
  }

  // Get predicate
  if (tokens_.empty()) {
    LOGE << "invalid NOT-condition: expected predicate in line "
         << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> predicate = GetPredicate();
  ASTUtils::Link(node, predicate);

  return node;
}
NodePtr<INode> Parser::GetPredicate() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kPredicate);
  NodePtr<INode> lhs = GetExpression();
  ASTUtils::Link(node, lhs);

  int line = PeekToken()->line;
  if (tokens_.empty()) {
    LOGE << "invalid predicate: expected "
            "binary operator in line " << line;
    end(EXIT_FAILURE);
  }
  if (NodeDataClassifier::IsBinaryOperator(PeekToken())) {
    line = PeekToken()->line;
    NodePtr<INode> bin_operator = NextToken();

    ASTUtils::Link(node, bin_operator);
  } else {
    LOGE << "invalid predicate in line "
         << line << ": operator should be binary";
    end(EXIT_FAILURE);
  }

  if (tokens_.empty()) {
    LOGE << "invalid predicate: expected "
            "right hand side expression in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> rhs = GetExpression();
  ASTUtils::Link(node, rhs);

  return node;
}
NodePtr<INode> Parser::GetExpression() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kExpression);

  int line = PeekToken()->line;

  // Is it [table_name.] column ?
  if (NodeDataTypeClassifier::IsWord(PeekToken())) {
    NodePtr<INode> name = GetIdentifier();
    ASTUtils::Link(node, name);

    if (!tokens_.empty() && NodeDataClassifier::IsDot(PeekToken())) {
      NodePtr<INode> dot = GetIdentifiers();
      ASTUtils::Link(node, dot);
    }

    return node;
  }

  // Is it unary operator ?
  if (NodeDataClassifier::IsUnaryOperator(PeekToken())) {
    NodePtr<INode> u_operator, expression;
    u_operator = NextToken();

    ASTUtils::Link(node, u_operator);

    if (tokens_.empty()) {
      LOGE <<
           "invalid expression in line "
           << line << ": an unary operator without an operand";
      end(EXIT_FAILURE);
    }
    expression = GetExpression();
    ASTUtils::Link(node, expression);

    return node;
  }

  // Is it (expression) ?
  if (NodeDataClassifier::IsOpeningRoundBracket(PeekToken())) {
    NextToken();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    line = PeekToken()->line;
    node = GetExpression();

    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    NextToken();

    return node;
  }

  // Is it string ?
  if (NodeDataClassifier::IsQuote(PeekToken())) {
    NextToken();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": invalid string";
      end(EXIT_FAILURE);
    }
    NodePtr<INode> str = GetString();
    ASTUtils::Link(node, str);
  }

  // So, it is Math expression
  NodePtr<INode> expression = GetMathExpression();
  ASTUtils::Link(node, expression);

  return node;
}

} // scc::parser
