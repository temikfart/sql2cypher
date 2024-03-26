#include "SCC/syntax_analyzer.h"

namespace scc::parser {

using namespace ast;

StmtType SyntaxAnalyzer::GetDMLStType() {
  StmtType DMLStType = StmtType::kNone; // invalid value

  // Get first token
  int line = this->peek_first_token()->line;
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->data;
  this->pop_first_token();

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
std::shared_ptr<INode> SyntaxAnalyzer::GetDMLSt() {
  LOGD << "getting DML statement...";
  std::shared_ptr<INode> node, statement;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kDmlStmt;

  int line = this->peek_first_token()->line;
  switch (this->GetDMLStType()) {
    case StmtType::kUpdateStmt:
      statement = this->GetUpdateSt();
      LOGD << "got UPDATE statement";
      break;
    case StmtType::kDeleteStmt:
      statement = this->GetDeleteSt();
      LOGD << "got DELETE statement";
      break;
    case StmtType::kInsertStmt:
      statement = this->GetInsertSt();
      LOGD << "got INSERT statement";
      break;
    default:
      LOGE << "unknown DML statement near line " << line;
      end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, statement);

  return node;
}

// DML Statements

std::shared_ptr<INode> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<INode> node;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kInsertStmt;

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<INode> node;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kDeleteStmt;

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<INode> node;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kUpdateStmt;

  return node;
}

// DML Basic statements

std::shared_ptr<INode> SyntaxAnalyzer::GetCondition() {
  std::shared_ptr<INode> node, OR_condition;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kCondition;

  OR_condition = this->GetORCondition();

  SyntaxAnalyzer::MakeKinship(node, OR_condition);

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetORCondition() {
  std::shared_ptr<INode> node, AND_condition, next_AND_conditions;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kORCondition;

  AND_condition = this->GetANDCondition();

  SyntaxAnalyzer::MakeKinship(node, AND_condition);

  int line = this->peek_first_token()->line;
  if (!tokens_.empty()
      && SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->data == "OR") {
      this->pop_first_token();

      if (tokens_.empty()) {
        LOGE << "invalid OR-condition: expected AND-condition "
                   "after the \'OR\' logical operator in line " << line;
        end(EXIT_FAILURE);
      }
      next_AND_conditions = this->GetANDCondition();
      SyntaxAnalyzer::MakeKinship(node, next_AND_conditions);
    }
  }

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetANDCondition() {
  std::shared_ptr<INode> node, NOT_condition, next_NOT_conditions;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kANDCondition;

  NOT_condition = this->GetNOTCondition();

  SyntaxAnalyzer::MakeKinship(node, NOT_condition);

  int line = this->peek_first_token()->line;
  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
      std::shared_ptr<StringNode> tmp =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (tmp->data == "AND") {
        this->pop_first_token();

        if (tokens_.empty()) {
          LOGE << "invalid AND-condition: expected NOT-condition "
                     "after the \\'AND\\' operator in line " << line;
          end(EXIT_FAILURE);
        }
        next_NOT_conditions = this->GetNOTCondition();
        SyntaxAnalyzer::MakeKinship(node, next_NOT_conditions);
      }
    }
  }

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetNOTCondition() {
  std::shared_ptr<INode> node, NOT_operator, predicate;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kNOTCondition;

  // Get NOT if present
  int line = this->peek_first_token()->line;
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->data == "NOT") {
      NOT_operator = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, NOT_operator);
    }
  }

  // Get predicate
  if (tokens_.empty()) {
    LOGE << "invalid NOT-condition: expected predicate in line "
        << line;
    end(EXIT_FAILURE);
  }
  predicate = this->GetPredicate();
  SyntaxAnalyzer::MakeKinship(node, predicate);

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetPredicate() {
  std::shared_ptr<INode> node, lhs, rhs, bin_operator;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kPredicate;

  int line = this->peek_first_token()->line;
  lhs = this->GetExpression();
  SyntaxAnalyzer::MakeKinship(node, lhs);

  if (tokens_.empty()) {
    LOGE << "invalid predicate: expected "
               "binary operator in line " << line;
    end(EXIT_FAILURE);
  }
  if (SyntaxAnalyzer::IsBinaryOperator(this->peek_first_token())) {
    line = this->peek_first_token()->line;
    bin_operator = this->get_first_token();

    SyntaxAnalyzer::MakeKinship(node, bin_operator);
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
  rhs = this->GetExpression();
  SyntaxAnalyzer::MakeKinship(node, rhs);

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetExpression() {
  std::shared_ptr<INode> node;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::kExpression;

  int line = this->peek_first_token()->line;

  // Is it [table_name.] column ?
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<INode> name, dot;

    name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, name);

    if (!tokens_.empty()) {
      if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
        dot = this->GetIdentifiers();

        SyntaxAnalyzer::MakeKinship(node, dot);
      }
    }

    return node;
  }

  // Is it unary operator ?
  if (SyntaxAnalyzer::IsUnaryOperator(this->peek_first_token())) {
    std::shared_ptr<INode> u_operator, expression;
    u_operator = this->get_first_token();

    SyntaxAnalyzer::MakeKinship(node, u_operator);

    if (tokens_.empty()) {
      LOGE <<
          "invalid expression in line "
              << line << ": an unary operator without an operand";
      end(EXIT_FAILURE);
    }
    expression = this->GetExpression();
    SyntaxAnalyzer::MakeKinship(node, expression);

    return node;
  }

  // Is it (expression) ?
  if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
          << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    line = this->peek_first_token()->line;
    node = this->GetExpression();

    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
          << line << ": bad bracket sequence \'(.\'";
      end(EXIT_FAILURE);
    }
    this->pop_first_token();

    return node;
  }

  // Is it string ?
  if (SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_.empty()) {
      LOGE << "invalid expression in line "
          << line << ": invalid string";
      end(EXIT_FAILURE);
    }
    std::shared_ptr<INode> str = this->GetString();
    SyntaxAnalyzer::MakeKinship(node, str);
  }

  // So, it is Math expression
  std::shared_ptr<INode> expression = this->GetMathExpression();
  SyntaxAnalyzer::MakeKinship(node, expression);

  return node;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetMathExpression() {
  std::shared_ptr<INode> node;

  // TODO: implement this PEG
  /* MathExpr   <-- Sum
   * Sum        <-- Product (('+' / '-') Product)*
   * Product    <-- Power (('*' / '/') Power)*
   * Power      <-- Value ('^' Power)?
   * Value      <-- [0-9]+ / '(' MathExpr ')'                   */

  node = this->GetMathSum();

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetMathSum() {
  std::shared_ptr<INode> product_1, op_node, product_2;

  int line = this->peek_first_token()->line;
  product_1 = this->GetMathProduct();

  if (!tokens_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("+" | "-")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->data;
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
      product_2 = this->GetMathProduct();

      SyntaxAnalyzer::MakeKinship(op_node, product_1);
      SyntaxAnalyzer::MakeKinship(op_node, product_2);
      product_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return product_1;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetMathProduct() {
  std::shared_ptr<INode> power_1, op_node, power_2;

  int line = this->peek_first_token()->line;
  power_1 = this->GetMathPower();

  if (!tokens_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("*" | "/")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->data;
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
      power_2 = this->GetMathPower();

      SyntaxAnalyzer::MakeKinship(op_node, power_1);
      SyntaxAnalyzer::MakeKinship(op_node, power_2);
      power_1 = op_node;

      if (tokens_.empty()) {
        break;
      }
    }
  }

  return power_1;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetMathPower() {
  std::shared_ptr<INode> power, degree_op, degree;

  int line = this->peek_first_token()->line;
  power = this->GetMathValue();

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      std::shared_ptr<StringNode> op_node =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (op_node->data == "^") {
        degree_op = this->get_first_token();

        if (tokens_.empty()) {
          LOGE << "invalid Math expression in line "
              << line << ": power missing";
          end(EXIT_FAILURE);
        }
        degree = this->GetMathPower();
        SyntaxAnalyzer::MakeKinship(degree_op, power);
        SyntaxAnalyzer::MakeKinship(degree_op, degree);

        return degree_op;
      }
    }
  }

  return power;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetMathValue() {
  std::shared_ptr<INode> value;

  int line = this->peek_first_token()->line;
  if (SyntaxAnalyzer::IsNumber(this->peek_first_token())) {
    value = this->get_first_token();
  } else if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();

    if (tokens_.empty()) {
      LOGE << "invalid Math expression: "
                 "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
    line = this->peek_first_token()->line;
    value = this->GetMathExpression();

    if (SyntaxAnalyzer::IsClosingRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    } else {
      LOGE << "invalid Math expression: "
                 "expected closing round bracket in line " << line;
      end(EXIT_FAILURE);
    }
  }

  return value;
}

} // scc::parser
