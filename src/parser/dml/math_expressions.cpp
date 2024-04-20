#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::GetMathExpression() {
  NodePtr<INode> node;

  // TODO: implement this PEG
  /* MathExpr   <-- Sum
   * Sum        <-- Product (('+' / '-') Product)*
   * Product    <-- Power (('*' / '/') Power)*
   * Power      <-- Value ('^' Power)?
   * Value      <-- [0-9]+ / '(' MathExpr ')'                   */

  node = GetMathSum();

  return node;
}
NodePtr<INode> Parser::GetMathSum() {
  NodePtr<INode> product_1, op_node, product_2;

  int line = PeekToken()->line;
  product_1 = GetMathProduct();

  if (!tokens_.empty()) {
    while (NodeDataTypeClassifier::IsOperator(PeekToken())) {
      // Get ("+" | "-")
      op_node = NextToken();
      std::string operator_str = ASTUtils::CastToNodeType<StringNode>(op_node)->data;
      if (operator_str != "+" || operator_str != "-") {
        LOGE << "invalid Math expression in line "
             << line << ": wrong operator \'" << operator_str << "\'";
        end(EXIT_FAILURE);
      }

      // Get next powers
      if (tokens_.empty()) {
        LOGE << "invalid Math expression: expected second operand for "
                "the \'" << operator_str << "\' in line " << line;
        end(EXIT_FAILURE);
      }
      product_2 = GetMathProduct();

      ASTUtils::Link(op_node, product_1);
      ASTUtils::Link(op_node, product_2);
      product_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return product_1;
}
NodePtr<INode> Parser::GetMathProduct() {
  NodePtr<INode> power_1, op_node, power_2;

  int line = PeekToken()->line;
  power_1 = GetMathPower();

  if (!tokens_.empty()) {
    while (NodeDataTypeClassifier::IsOperator(PeekToken())) {
      // Get ("*" | "/")
      op_node = NextToken();
      std::string operator_str = ASTUtils::CastToNodeType<StringNode>(op_node)->data;
      if (operator_str != "*" || operator_str != "/") {
        LOGE << "invalid Math expression in line "
             << line << ": wrong operator \'" << operator_str << "\'";
        end(EXIT_FAILURE);
      }

      // Get next powers
      if (tokens_.empty()) {
        LOGE << "invalid Math expression: expected second operand for "
                "the \'" << operator_str << "\' in line " << line;
        end(EXIT_FAILURE);
      }
      power_2 = GetMathPower();

      ASTUtils::Link(op_node, power_1);
      ASTUtils::Link(op_node, power_2);
      power_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return power_1;
}
NodePtr<INode> Parser::GetMathPower() {
  NodePtr<INode> power, degree_op, degree;

  int line = PeekToken()->line;
  power = GetMathValue();

  if (!tokens_.empty()) {
    if (NodeDataTypeClassifier::IsOperator(PeekToken())) {
      NodePtr<StringNode> op_node = ASTUtils::CastToNodeType<StringNode>(PeekToken());
      if (op_node->data == "^") {
        degree_op = NextToken();

        if (tokens_.empty()) {
          LOGE << "invalid Math expression in line "
               << line << ": power missing";
          end(EXIT_FAILURE);
        }
        degree = GetMathPower();
        ASTUtils::Link(degree_op, power);
        ASTUtils::Link(degree_op, degree);

        return degree_op;
      }
    }
  }

  return power;
}
NodePtr<INode> Parser::GetMathValue() {
  NodePtr<INode> value;

  int line = PeekToken()->line;
  if (NodeDataTypeClassifier::IsNumber(PeekToken())) {
    value = NextToken();
  } else if (NodeDataClassifier::IsOpeningRoundBracket(PeekToken())) {
    NextToken();

    if (tokens_.empty()) {
      LOGE << "invalid Math expression: "
              "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
    line = PeekToken()->line;
    value = GetMathExpression();

    if (NodeDataClassifier::IsClosingRoundBracket(PeekToken())) {
      NextToken();
    } else {
      LOGE << "invalid Math expression: "
              "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
  }

  return value;
}

} // scc::parser
