#include "SCC/syntax_analyzer.h"

StatementType SyntaxAnalyzer::GetDMLStType() {
  StatementType DMLStType = StatementType::StTypeCount; // invalid value

  // Get first token
  int line = this->peek_first_token()->get_line();
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOG(ERROR, "body of the DDL statement is missed in line " << line);
    end(EXIT_FAILURE);
  }

  // Define next rule
  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { DMLStType = StatementType::updateStatement; }
  else if (is_DELETE) { DMLStType = StatementType::deleteStatement; }
  else if (is_INSERT) { DMLStType = StatementType::insertStatement; }
  else {
    LOG(ERROR, "unknown DML statement type in line " << line);
    end(EXIT_FAILURE);
  }

  return DMLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  LOG(TRACE, "getting DML statement...");
  std::shared_ptr<Node> node, statement;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::dmlStatement);

  int line = this->peek_first_token()->get_line();
  switch (this->GetDMLStType()) {
    case StatementType::updateStatement:
      statement = this->GetUpdateSt();
      LOG(TRACE, "got UPDATE statement");
      break;
    case StatementType::deleteStatement:
      statement = this->GetDeleteSt();
      LOG(TRACE, "got DELETE statement");
      break;
    case StatementType::insertStatement:
      statement = this->GetInsertSt();
      LOG(TRACE, "got INSERT statement");
      break;
    default:
      LOG(ERROR, "unknown DML statement near line " << line);
      end(EXIT_FAILURE);
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

  int line = this->peek_first_token()->get_line();
  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->get_data() == "OR") {
      this->pop_first_token();

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid OR-condition: expected AND-condition "
                   "after the \'OR\' logical operator in line " << line);
        end(EXIT_FAILURE);
      }
      next_AND_conditions = this->GetANDCondition();
      SyntaxAnalyzer::MakeKinship(node, next_AND_conditions);
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

  int line = this->peek_first_token()->get_line();
  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
      std::shared_ptr<StringNode> tmp =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (tmp->get_data() == "AND") {
        this->pop_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid AND-condition: expected NOT-condition "
                     "after the \\'AND\\' operator in line " << line);
          end(EXIT_FAILURE);
        }
        next_NOT_conditions = this->GetNOTCondition();
        SyntaxAnalyzer::MakeKinship(node, next_NOT_conditions);
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
  int line = this->peek_first_token()->get_line();
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
    LOG(ERROR, "invalid NOT-condition: expected predicate in line "
        << line);
    end(EXIT_FAILURE);
  }
  predicate = this->GetPredicate();
  SyntaxAnalyzer::MakeKinship(node, predicate);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPredicate() {
  std::shared_ptr<Node> node, lhs, rhs, bin_operator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::predicate);

  int line = this->peek_first_token()->get_line();
  lhs = this->GetExpression();
  SyntaxAnalyzer::MakeKinship(node, lhs);

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: expected "
               "binary operator in line " << line);
    end(EXIT_FAILURE);
  }
  if (SyntaxAnalyzer::IsBinaryOperator(this->peek_first_token())) {
    line = this->peek_first_token()->get_line();
    bin_operator = this->get_first_token();

    SyntaxAnalyzer::MakeKinship(node, bin_operator);
  } else {
    LOG(ERROR, "invalid predicate in line "
        << line << ": operator should be binary");
    end(EXIT_FAILURE);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: expected "
               "right hand side expression in line " << line);
    end(EXIT_FAILURE);
  }
  rhs = this->GetExpression();
  SyntaxAnalyzer::MakeKinship(node, rhs);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetExpression() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::expression);

  int line = this->peek_first_token()->get_line();

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
          "invalid expression in line "
              << line << ": an unary operator without an operand");
      end(EXIT_FAILURE);
    }
    expression = this->GetExpression();
    SyntaxAnalyzer::MakeKinship(node, expression);

    return node;
  }

  // Is it (expression) ?
  if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression in line "
          << line << ": bad bracket sequence \'(.\'");
      end(EXIT_FAILURE);
    }
    line = this->peek_first_token()->get_line();
    node = this->GetExpression();

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression in line "
          << line << ": bad bracket sequence \'(.\'");
      end(EXIT_FAILURE);
    }
    this->pop_first_token();

    return node;
  }

  // Is it string ?
  if (SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression in line "
          << line << ": invalid string");
      end(EXIT_FAILURE);
    }
    std::shared_ptr<Node> str = this->GetString();
    SyntaxAnalyzer::MakeKinship(node, str);
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

  int line = this->peek_first_token()->get_line();
  product_1 = this->GetMathProduct();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("+" | "-")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "+" || operator_str != "-") {
        LOG(ERROR, "invalid Math expression in line "
            << line << ": wrong operator \'" << operator_str << "\'");
        end(EXIT_FAILURE);
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: expected second operand for "
                   "the \'" << operator_str << "\' in line " << line);
        end(EXIT_FAILURE);
      }
      product_2 = this->GetMathProduct();

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

  int line = this->peek_first_token()->get_line();
  power_1 = this->GetMathPower();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("*" | "/")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "*" || operator_str != "/") {
        LOG(ERROR, "invalid Math expression in line "
            << line << ": wrong operator \'" << operator_str << "\'");
        end(EXIT_FAILURE);
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: expected second operand for "
                   "the \'" << operator_str << "\' in line " << line);
        end(EXIT_FAILURE);
      }
      power_2 = this->GetMathPower();

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

  int line = this->peek_first_token()->get_line();
  power = this->GetMathValue();

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      std::shared_ptr<StringNode> op_node =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (op_node->get_data() == "^") {
        degree_op = this->get_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid Math expression in line "
              << line << ": power missing");
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
std::shared_ptr<Node> SyntaxAnalyzer::GetMathValue() {
  std::shared_ptr<Node> value;

  int line = this->peek_first_token()->get_line();
  if (SyntaxAnalyzer::IsNumber(this->peek_first_token())) {
    value = this->get_first_token();
  } else if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid Math expression: "
                 "expected closing round bracket in line " << line);
      end(EXIT_FAILURE);
    }
    line = this->peek_first_token()->get_line();
    value = this->GetMathExpression();

    if (SyntaxAnalyzer::IsClosingRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    } else {
      LOG(ERROR, "invalid Math expression: "
                 "expected closing round bracket in line " << line);
      end(EXIT_FAILURE);
    }
  }

  return value;
}
