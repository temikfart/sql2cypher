#include "SCC/syntax_analyzer.h"

StatementType SyntaxAnalyzer::GetDMLStType() {
  StatementType DMLStType = StatementType::StTypeCount; // invalid value

  // Get first token
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  // Define next rule
  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { DMLStType = StatementType::updateStatement; }
  else if (is_DELETE) { DMLStType = StatementType::deleteStatement; }
  else if (is_INSERT) { DMLStType = StatementType::insertStatement; }
  else {
    LOG(ERROR, "unknown DML statement type");
    exit(EXIT_FAILURE);
  }

  return DMLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node, statement;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::dmlStatement);

  switch (this->GetDMLStType()) {
    case StatementType::updateStatement:
      statement = this->GetUpdateSt();
      break;
    case StatementType::deleteStatement:
      statement = this->GetDeleteSt();
      break;
    case StatementType::insertStatement:
      statement = this->GetInsertSt();
      break;
    default:
      LOG(ERROR, "unknown DML statement");
      exit(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, statement);

  return node;
}

// DML Statements

std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::insertStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::deleteStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::updateStatement);

  return node;
}

// DML Basic statements

std::shared_ptr<Node> SyntaxAnalyzer::GetCondition() {
  std::shared_ptr<Node> node, OR_condition;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::condition);

  OR_condition = this->GetORCondition();

  SyntaxAnalyzer::MakeKinship(node, OR_condition);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetORCondition() {
  std::shared_ptr<Node> node, AND_condition, next_AND_conditions;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ORcondition);

  AND_condition = this->GetANDCondition();

  SyntaxAnalyzer::MakeKinship(node, AND_condition);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->get_data() == "OR") {
      this->pop_first_token();

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid OR-condition: AND-condition "
                   "after the \'OR\' operator is missed");
        return node;
      } else {
        next_AND_conditions = this->GetANDCondition();

        SyntaxAnalyzer::MakeKinship(node, next_AND_conditions);
      }
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetANDCondition() {
  std::shared_ptr<Node> node, NOT_condition, next_NOT_conditions;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ANDcondition);

  NOT_condition = this->GetNOTCondition();

  SyntaxAnalyzer::MakeKinship(node, NOT_condition);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
      std::shared_ptr<StringNode> tmp =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (tmp->get_data() == "AND") {
        this->pop_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid AND-condition: NOT-condition "
                     "after the \\'AND\\' operator is missed");
          return node;
        } else {
          next_NOT_conditions = this->GetNOTCondition();

          SyntaxAnalyzer::MakeKinship(node, next_NOT_conditions);
        }
      }
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetNOTCondition() {
  std::shared_ptr<Node> node, NOT_operator, predicate;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::NOTcondition);

  // Get NOT if present
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->get_data() == "NOT") {
      NOT_operator = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, NOT_operator);
    }
  }

  // Get predicate
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid NOT-condition: predicate is missed");
    return node;
  } else {
    predicate = this->GetPredicate();

    SyntaxAnalyzer::MakeKinship(node, predicate);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPredicate() {
  std::shared_ptr<Node> node, lhs, rhs, bin_operator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::predicate);

  lhs = this->GetExpression();

  SyntaxAnalyzer::MakeKinship(node, lhs);

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: binary operator is missed");
    return node;
  } else {
    if (SyntaxAnalyzer::IsBinaryOperator(this->peek_first_token())) {
      bin_operator = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, bin_operator);
    } else {
      LOG(ERROR, "invalid predicate: operator should be binary");
      return node;
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: right hand side expression is missed");
    return node;
  } else {
    rhs = this->GetExpression();

    SyntaxAnalyzer::MakeKinship(node, rhs);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetExpression() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::expression);

  // Is it [table_name.] column ?
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<Node> name, dot;

    name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, name);

    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
        dot = this->GetIdentifiers();

        SyntaxAnalyzer::MakeKinship(node, dot);
      }
    }

    return node;
  }

  // Is it unary operator ?
  if (SyntaxAnalyzer::IsUnaryOperator(this->peek_first_token())) {
    std::shared_ptr<Node> u_operator, expression;
    u_operator = this->get_first_token();

    SyntaxAnalyzer::MakeKinship(node, u_operator);

    if (tokens_array_.empty()) {
      LOG(ERROR,
          "invalid expression: an unary operator without an operand");
      return node;
    } else {
      expression = this->GetExpression();

      SyntaxAnalyzer::MakeKinship(node, expression);
    }

    return node;
  }

  // Is it (expression) ?
  if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: bad bracket sequence \'(.\'");
      return node;
    } else {
      node = this->GetExpression();
    }

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: bad bracket sequence \'(.\'");
      return node;
    } else {
      this->pop_first_token();
    }

    return node;
  }

  // Is it string ?
  if (SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: invalid string");
      return node;
    } else {
      std::shared_ptr<Node> str = this->GetString();

      SyntaxAnalyzer::MakeKinship(node, str);
    }
  }

  // So, it is Math expression
  std::shared_ptr<Node> expression = this->GetMathExpression();

  SyntaxAnalyzer::MakeKinship(node, expression);

  return node;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetMathExpression() {
  std::shared_ptr<Node> node;

  // TODO: implement this PEG
  /* MathExpr   <-- Sum
   * Sum        <-- Product (('+' / '-') Product)*
   * Product    <-- Power (('*' / '/') Power)*
   * Power      <-- Value ('^' Power)?
   * Value      <-- [0-9]+ / '(' MathExpr ')'                   */

  node = this->GetMathSum();

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathSum() {
  std::shared_ptr<Node> product_1, op_node, product_2;

  product_1 = this->GetMathProduct();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("+" | "-")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "+" || operator_str != "-") {
        LOG(ERROR, "invalid Math expression: wrong operator \'"
            << operator_str << "\'");
        return product_1;
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: second operand for the \'"
            << operator_str << "\' is missed");
        return product_1;
      } else {
        product_2 = this->GetMathProduct();
      }

      SyntaxAnalyzer::MakeKinship(op_node, product_1);
      SyntaxAnalyzer::MakeKinship(op_node, product_2);
      product_1 = op_node;

      if (tokens_array_.empty()) {
        break;
      }
    }
  }

  return product_1;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathProduct() {
  std::shared_ptr<Node> power_1, op_node, power_2;

  power_1 = this->GetMathPower();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("*" | "/")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "*" || operator_str != "/") {
        LOG(ERROR, "invalid Math expression: wrong operator \'"
            << operator_str << "\'");
        return power_1;
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: second operand for the \'"
            << operator_str << "\' is missed");
        return power_1;
      } else {
        power_2 = this->GetMathPower();
      }

      SyntaxAnalyzer::MakeKinship(op_node, power_1);
      SyntaxAnalyzer::MakeKinship(op_node, power_2);
      power_1 = op_node;

      if (tokens_array_.empty()) {
        break;
      }
    }
  }

  return power_1;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathPower() {
  std::shared_ptr<Node> power, degree_op, degree;

  power = this->GetMathValue();

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      std::shared_ptr<StringNode> op_node =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (op_node->get_data() == "^") {
        degree_op = this->get_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid Math expression: degree is missed");
          return power;
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
std::shared_ptr<Node> SyntaxAnalyzer::GetMathValue() {
  std::shared_ptr<Node> value;

  if (SyntaxAnalyzer::IsNumber(this->peek_first_token())) {
    value = this->get_first_token();
  } else if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid Math expression: closing round bracket is missed");
      return value;
    } else {
      value = this->GetMathExpression();
    }

    if (SyntaxAnalyzer::IsClosingRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    } else {
      LOG(ERROR, "invalid Math expression: closing round bracket is missed");
      return value;
    }
  }

  return value;
}
