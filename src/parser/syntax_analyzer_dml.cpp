#include "SCC/parser/syntax_analyzer.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType SyntaxAnalyzer::GetDMLStType() {
  StmtType DMLStType = StmtType::kNone; // invalid value

  // Get first token
  int line = peek_first_token()->line;
  std::string fst_kw = CastToNodeType<StringNode>(peek_first_token())->data;
  pop_first_token();

  if (tokens_.empty()) {
    LOGE << "body of the DDL statement is missed in line " << line;
    end(EXIT_FAILURE);
  }

  // Define next rule
  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { DMLStType = StmtType::kUpdateStmt; }
  else if (is_DELETE) { DMLStType = StmtType::kDeleteStmt; }
  else if (is_INSERT) { DMLStType = StmtType::kInsertStmt; }
  else {
    LOGE << "unknown DML statement type in line " << line;
    end(EXIT_FAILURE);
  }

  return DMLStType;
}
NodePtr<INode> SyntaxAnalyzer::GetDMLSt() {
  LOGD << "getting DML statement...";
  NodePtr<INode> node = CreateServiceNode(StmtType::kDmlStmt);

  int line = peek_first_token()->line;
  NodePtr<INode> statement;
  switch (GetDMLStType()) {
    case StmtType::kUpdateStmt:
      statement = GetUpdateSt();
      LOGD << "got UPDATE statement";
      break;
    case StmtType::kDeleteStmt:
      statement = GetDeleteSt();
      LOGD << "got DELETE statement";
      break;
    case StmtType::kInsertStmt:
      statement = GetInsertSt();
      LOGD << "got INSERT statement";
      break;
    default:
      LOGE << "unknown DML statement near line " << line;
      end(EXIT_FAILURE);
  }
  INode::Link(node, statement);

  return node;
}

// DML Statements

NodePtr<INode> SyntaxAnalyzer::GetInsertSt() {
  return CreateServiceNode(StmtType::kInsertStmt);
}
NodePtr<INode> SyntaxAnalyzer::GetDeleteSt() {
  return CreateServiceNode(StmtType::kDeleteStmt);
}
NodePtr<INode> SyntaxAnalyzer::GetUpdateSt() {
  return CreateServiceNode(StmtType::kUpdateStmt);
}

// DML Basic statements

NodePtr<INode> SyntaxAnalyzer::GetCondition() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kCondition);
  NodePtr<INode> OR_condition = GetORCondition();
  INode::Link(node, OR_condition);
  
  return node;
}
NodePtr<INode> SyntaxAnalyzer::GetORCondition() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kORCondition);
  NodePtr<INode> AND_condition = GetANDCondition();
  INode::Link(node, AND_condition);

  int line = peek_first_token()->line;
  if (!tokens_.empty()
      && SyntaxAnalyzer::IsWord(peek_first_token())) {
    NodePtr<StringNode> tmp = CastToNodeType<StringNode>(peek_first_token());
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
NodePtr<INode> SyntaxAnalyzer::GetANDCondition() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kANDCondition);
  NodePtr<INode> NOT_condition = GetNOTCondition();
  INode::Link(node, NOT_condition);

  int line = peek_first_token()->line;
  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsWord(peek_first_token())) {
      NodePtr<StringNode> tmp = CastToNodeType<StringNode>(peek_first_token());
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
NodePtr<INode> SyntaxAnalyzer::GetNOTCondition() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kNOTCondition);

  // Get NOT if present
  int line = peek_first_token()->line;
  if (SyntaxAnalyzer::IsWord(peek_first_token())) {
    NodePtr<StringNode> tmp = CastToNodeType<StringNode>(peek_first_token());
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
NodePtr<INode> SyntaxAnalyzer::GetPredicate() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kPredicate);
  NodePtr<INode> lhs = GetExpression();
  INode::Link(node, lhs);

  int line = peek_first_token()->line;
  if (tokens_.empty()) {
    LOGE << "invalid predicate: expected "
               "binary operator in line " << line;
    end(EXIT_FAILURE);
  }
  if (SyntaxAnalyzer::IsBinaryOperator(peek_first_token())) {
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
NodePtr<INode> SyntaxAnalyzer::GetExpression() {
  NodePtr<INode> node = CreateServiceNode(StmtType::kExpression);

  int line = peek_first_token()->line;

  // Is it [table_name.] column ?
  if (SyntaxAnalyzer::IsWord(peek_first_token())) {
    NodePtr<INode> name = GetIdentifier();
    INode::Link(node, name);

    if (!tokens_.empty()) {
      if (SyntaxAnalyzer::IsDot(peek_first_token())) {
        NodePtr<INode> dot = GetIdentifiers();

        INode::Link(node, dot);
      }
    }

    return node;
  }

  // Is it unary operator ?
  if (SyntaxAnalyzer::IsUnaryOperator(peek_first_token())) {
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
  if (SyntaxAnalyzer::IsOpeningRoundBracket(peek_first_token())) {
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
  if (SyntaxAnalyzer::IsSingleQuote(peek_first_token())
      || SyntaxAnalyzer::IsDoubleQuote(peek_first_token())) {
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

NodePtr<INode> SyntaxAnalyzer::GetMathExpression() {
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
NodePtr<INode> SyntaxAnalyzer::GetMathSum() {
  NodePtr<INode> product_1, op_node, product_2;

  int line = peek_first_token()->line;
  product_1 = GetMathProduct();

  if (!tokens_.empty()) {
    while (SyntaxAnalyzer::IsOperator(peek_first_token())) {
      // Get ("+" | "-")
      op_node = get_first_token();
      std::string operator_str = CastToNodeType<StringNode>(op_node)->data;
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

      INode::Link(op_node, product_1);
      INode::Link(op_node, product_2);
      product_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return product_1;
}
NodePtr<INode> SyntaxAnalyzer::GetMathProduct() {
  NodePtr<INode> power_1, op_node, power_2;

  int line = peek_first_token()->line;
  power_1 = GetMathPower();

  if (!tokens_.empty()) {
    while (SyntaxAnalyzer::IsOperator(peek_first_token())) {
      // Get ("*" | "/")
      op_node = get_first_token();
      std::string operator_str = CastToNodeType<StringNode>(op_node)->data;
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

      INode::Link(op_node, power_1);
      INode::Link(op_node, power_2);
      power_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return power_1;
}
NodePtr<INode> SyntaxAnalyzer::GetMathPower() {
  NodePtr<INode> power, degree_op, degree;

  int line = peek_first_token()->line;
  power = GetMathValue();

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsOperator(peek_first_token())) {
      NodePtr<StringNode> op_node = CastToNodeType<StringNode>(peek_first_token());
      if (op_node->data == "^") {
        degree_op = get_first_token();

        if (tokens_.empty()) {
          LOGE << "invalid Math expression in line "
              << line << ": power missing";
          end(EXIT_FAILURE);
        }
        degree = GetMathPower();
        INode::Link(degree_op, power);
        INode::Link(degree_op, degree);

        return degree_op;
      }
    }
  }

  return power;
}
NodePtr<INode> SyntaxAnalyzer::GetMathValue() {
  NodePtr<INode> value;

  int line = peek_first_token()->line;
  if (SyntaxAnalyzer::IsNumber(peek_first_token())) {
    value = get_first_token();
  } else if (SyntaxAnalyzer::IsOpeningRoundBracket(peek_first_token())) {
    pop_first_token();

    if (tokens_.empty()) {
      LOGE << "invalid Math expression: "
                 "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
    line = peek_first_token()->line;
    value = GetMathExpression();

    if (SyntaxAnalyzer::IsClosingRoundBracket(peek_first_token())) {
      pop_first_token();
    } else {
      LOGE << "invalid Math expression: "
                 "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
  }

  return value;
}

} // scc::parser
