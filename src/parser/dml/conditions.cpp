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
  INode::Link(node, OR_condition);

  return node;
}
NodePtr<INode> Parser::GetORCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kORCondition);
  NodePtr<INode> AND_condition = GetANDCondition();
  INode::Link(node, AND_condition);

  int line = peek_first_token()->line;
  if (!tokens_.empty() && NodeDataTypeClassifier::IsWord(peek_first_token())) {
    NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(peek_first_token());
    if (tmp->data == "OR") {
      pop_first_token();

      if (tokens_.empty()) {
        LOGE << "invalid OR-condition: expected AND-condition "
                "after the \'OR\' logical operator in line " << line;
        end(EXIT_FAILURE);
      }
      NodePtr<INode> next_AND_conditions = GetANDCondition();
      INode::Link(node, next_AND_conditions);
    }
  }

  return node;
}
NodePtr<INode> Parser::GetANDCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kANDCondition);
  NodePtr<INode> NOT_condition = GetNOTCondition();
  INode::Link(node, NOT_condition);

  int line = peek_first_token()->line;
  if (!tokens_.empty()) {
    if (NodeDataTypeClassifier::IsWord(peek_first_token())) {
      NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(peek_first_token());
      if (tmp->data == "AND") {
        pop_first_token();

        if (tokens_.empty()) {
          LOGE << "invalid AND-condition: expected NOT-condition "
                  "after the \\'AND\\' operator in line " << line;
          end(EXIT_FAILURE);
        }
        NodePtr<INode> next_NOT_conditions = GetNOTCondition();
        INode::Link(node, next_NOT_conditions);
      }
    }
  }

  return node;
}
NodePtr<INode> Parser::GetNOTCondition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kNOTCondition);

  // Get NOT if present
  int line = peek_first_token()->line;
  if (NodeDataTypeClassifier::IsWord(peek_first_token())) {
    NodePtr<StringNode> tmp = ASTUtils::CastToNodeType<StringNode>(peek_first_token());
    if (tmp->data == "NOT") {
      NodePtr<INode> NOT_operator = get_first_token();

      INode::Link(node, NOT_operator);
    }
  }

  // Get predicate
  if (tokens_.empty()) {
    LOGE << "invalid NOT-condition: expected predicate in line "
         << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> predicate = GetPredicate();
  INode::Link(node, predicate);

  return node;
}
NodePtr<INode> Parser::GetPredicate() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kPredicate);
  NodePtr<INode> lhs = GetExpression();
  INode::Link(node, lhs);

  int line = peek_first_token()->line;
  if (tokens_.empty()) {
    LOGE << "invalid predicate: expected "
            "binary operator in line " << line;
    end(EXIT_FAILURE);
  }
  if (NodeDataClassifier::IsBinaryOperator(peek_first_token())) {
    line = peek_first_token()->line;
    NodePtr<INode> bin_operator = get_first_token();

    INode::Link(node, bin_operator);
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
  INode::Link(node, rhs);

  return node;
}
NodePtr<INode> Parser::GetExpression() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kExpression);

  int line = peek_first_token()->line;

  // Is it [table_name.] column ?
  if (NodeDataTypeClassifier::IsWord(peek_first_token())) {
    NodePtr<INode> name = GetIdentifier();
    INode::Link(node, name);

    if (!tokens_.empty() && NodeDataClassifier::IsDot(peek_first_token())) {
      NodePtr<INode> dot = GetIdentifiers();
      INode::Link(node, dot);
    }

    return node;
  }

  // Is it unary operator ?
  if (NodeDataClassifier::IsUnaryOperator(peek_first_token())) {
    NodePtr<INode> u_operator, expression;
    u_operator = get_first_token();

    INode::Link(node, u_operator);

    if (tokens_.empty()) {
      LOGE <<
           "invalid expression in line "
           << line << ": an unary operator without an operand";
      end(EXIT_FAILURE);
    }
    expression = GetExpression();
    INode::Link(node, expression);

    return node;
  }

  // Is it (expression) ?
  if (NodeDataClassifier::IsOpeningRoundBracket(peek_first_token())) {
    pop_first_token();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    line = peek_first_token()->line;
    node = GetExpression();

    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    pop_first_token();

    return node;
  }

  // Is it string ?
  if (NodeDataClassifier::IsQuote(peek_first_token())) {
    pop_first_token();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
           << line << ": invalid string";
      end(EXIT_FAILURE);
    }
    NodePtr<INode> str = GetString();
    INode::Link(node, str);
  }

  // So, it is Math expression
  NodePtr<INode> expression = GetMathExpression();
  INode::Link(node, expression);

  return node;
}

} // scc::parser
