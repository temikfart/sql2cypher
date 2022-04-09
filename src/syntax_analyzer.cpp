#include "SCC/syntax_analyzer.h"

std::shared_ptr<Node> SyntaxAnalyzer::Analyze(
    std::deque<std::shared_ptr<Node>> tokens_array) {
  tokens_array_ = std::move(tokens_array);

  if (tokens_array_.empty()) {
    LOG(TRACE, "empty tokens array");
    return nullptr;
  } else {
    std::shared_ptr<Node> root, query, separator;
    root = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
    root->set_st_type(StatementType::Program);

    query = this->GetDL();

    SyntaxAnalyzer::MakeKinship(root, query);

    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
        separator = this->General();

        SyntaxAnalyzer::MakeKinship(root, separator);
      }
    }
    return root;
  }
}

std::shared_ptr<Node> SyntaxAnalyzer::General() {
  std::shared_ptr<Node> separator, query, next_queries;

  separator = this->get_first_token();

  if (!tokens_array_.empty()) {
    query = this->GetDL();

    SyntaxAnalyzer::MakeKinship(separator, query);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
      next_queries = this->General();

      SyntaxAnalyzer::MakeKinship(separator, next_queries);
    }
  }

  return separator;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> node = std::dynamic_pointer_cast<Node>(
      std::make_shared<RootNode>());
  node->set_st_type(StatementType::query);

  auto first_token = this->peek_first_token();
  this->ValidateIsWord(first_token);

  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(first_token);

  std::string key_word = key_word_node->get_data();

  bool is_CREATE = key_word == "CREATE";
  bool is_ALTER = key_word == "ALTER";
  bool is_DROP = key_word == "DROP";

  bool is_UPDATE = key_word == "UPDATE";
  bool is_DELETE = key_word == "DELETE";
  bool is_INSERT = key_word == "INSERT";

  if (is_CREATE || is_ALTER || is_DROP) {             // It is DDL Statement
    node = this->GetDDLSt();
  } else if (is_UPDATE || is_DELETE || is_INSERT) {    // It is DML Statement
    node = this->GetDMLSt();
  } else {
    LOG(TRACE, "tokens array does not contain DDL or DML query");
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDDLSt() {
  std::shared_ptr<Node> node, child;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::ddlStatement);

  // Get first token
  std::shared_ptr<StringNode> first_key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

  // Get second token
  if (tokens_array_.empty()) {
    return node;
  }
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> second_key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

  // Define next rule
  std::string fst_kw = first_key_word_node->get_data();
  std::string snd_kw = second_key_word_node->get_data();

  bool is_CREATE = fst_kw == "CREATE";
  bool is_ALTER = fst_kw == "ALTER";
  bool is_DROP = fst_kw == "DROP";

  bool is_DATABASE = snd_kw == "DATABASE";
  bool is_TABLE = snd_kw == "TABLE";

  if (is_CREATE) {
    if (is_DATABASE) { child = this->GetCreateDatabaseSt(); }
    else if (is_TABLE) { child = this->GetCreateTableSt(); }
    else {
      LOG(ERROR, "unknown CREATE DDL statement");
    }
  } else if (is_ALTER) {
    if (is_TABLE) { child = this->GetAlterTableSt(); }
    else {
      LOG(ERROR, "unknown ALTER DDL statement");
    }
  } else if (is_DROP) {
    if (is_DATABASE) { child = this->GetDropDatabaseSt(); }
    else if (is_TABLE) { child = this->GetDropTableSt(); }
    else {
      LOG(ERROR, "unknown DROP DDL statement");
    }
  }

  SyntaxAnalyzer::MakeKinship(node, child);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node, child;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::dmlStatement);

  // Get first token
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

  // Define next rule
  std::string fst_kw = key_word_node->get_data();

  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { child = this->GetUpdateSt(); }
  else if (is_DELETE) { child = this->GetDeleteSt(); }
  else if (is_INSERT) { child = this->GetInsertSt(); }
  else {
    LOG(ERROR, "unknown DML statement");
  }

  SyntaxAnalyzer::MakeKinship(node, child);

  return node;
}

// DDL
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::createDatabaseStatement);

  if (tokens_array_.empty()) {
    LOG(ERROR, "databaseName is missed");
  } else {
    this->ValidateIsWord(this->peek_first_token());
    database_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, database_name);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateTableSt() {
  std::shared_ptr<Node> node, table_name, table_definition;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::createTableStatement);

  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
    return node;
  } else {
    // Get tableName
    this->ValidateIsWord(this->peek_first_token());
    table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, table_name);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "table definition is missed");
    return node;
  } else {
    table_definition = this->GetTableDefinition();

    SyntaxAnalyzer::MakeKinship(node, table_definition);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetAlterTableSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::alterTableStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::dropDatabaseStatement);

  // First database_name
  if (tokens_array_.empty()) {
    LOG(ERROR, "databaseName is missed");
  } else {
    this->ValidateIsWord(this->peek_first_token());
    database_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, database_name);
  }

  if (!tokens_array_.empty()) {
    // TODO: get ',' and other databaseNames
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropTableSt() {
  std::shared_ptr<Node> node, table_name;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::dropTableStatement);

  // First tableName
  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
  } else {
    this->ValidateIsWord(peek_first_token());
    table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, table_name);
  }

  if (!tokens_array_.empty()) {
    // TODO: get ',' and other tableNames
  }

  return node;
}

// DML
std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::insertStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::deleteStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::updateStatement);

  return node;
}

// Basic
std::shared_ptr<Node> SyntaxAnalyzer::GetTableDefinition() {
  std::shared_ptr<Node> node, argument, separator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::tableDefinition);

  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());

  std::string key_word = key_word_node->get_data();
  bool is_tableConstraint =
      key_word == "CONSTRAINT"
          || key_word == "PRIMARY"
          || key_word == "FOREIGN";

  if (is_tableConstraint) {
    argument = this->GetTableConstraint();
  } else {
    argument = this->GetColumnDefinition();
  }

  SyntaxAnalyzer::MakeKinship(node, argument);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetTableDefinition();

      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<Node> node, column_name, datatype;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::columnDefinition);

  // Get columnName
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(node, column_name);

  // Get datatype
  if (tokens_array_.empty()) {
    LOG(ERROR, "column datatype is missed");
    return node;
  } else {
    datatype = this->GetDataType();

    SyntaxAnalyzer::MakeKinship(node, datatype);
  }

  // Get other options
  if (!tokens_array_.empty()) {
    // TODO: get options such as IDENTITY or (NOT) NULL
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDataType() {
  this->ValidateIsWord(this->peek_first_token());

  std::shared_ptr<StringNode> node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

  std::string datatype = node->get_data();
  StatementType SQL_datatype;
  if (datatype == "int") { SQL_datatype = StatementType::SQL_int; }
  else if (datatype == "float") { SQL_datatype = StatementType::SQL_float; }
  else if (datatype == "char") { SQL_datatype = StatementType::SQL_char; }
  else if (datatype == "varchar") { SQL_datatype = StatementType::SQL_varchar; }
  else {
    LOG(ERROR, "invalid column datatype");
    return node;
  }

  node->set_st_type(SQL_datatype);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetTableConstraint() {
  std::shared_ptr<Node> node, constraint_name, key;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::tableConstraint);

  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<Node> key_word_node = this->peek_first_token();

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(key_word_node)->get_data();
  bool is_full_construction = key_word == "CONSTRAINT";
  if (is_full_construction) {
    this->pop_first_token();
    SyntaxAnalyzer::MakeKinship(node, key_word_node);

    if (tokens_array_.empty()) {
      LOG(ERROR, "constraint name is missed");
      return node;
    } else {
      this->ValidateIsWord(this->peek_first_token());
      constraint_name = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, constraint_name);
    }
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "constraint is missed");
    return node;
  } else {
    // Get PRIMARY | FOREIGN
    this->ValidateIsWord(this->peek_first_token());
    std::shared_ptr<StringNode> kind_of_constraint_node =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    this->pop_first_token();
    std::string kind_of_key = kind_of_constraint_node->get_data();

    // Get KEY
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid constraint definition");
      return node;
    }
    this->ValidateIsWord(this->peek_first_token());
    std::shared_ptr<StringNode> KEY_key_word =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    this->pop_first_token();

    if (kind_of_key == "PRIMARY") {
      key = this->GetPrimaryKey();
    } else if (kind_of_key == "FOREIGN") {
      key = this->GetForeignKey();
    } else {
      LOG(ERROR, "unknown kind of constraint: " << kind_of_key);
      return node;
    }

    SyntaxAnalyzer::MakeKinship(node, key);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<Node> node, column_name, separator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::primaryKey);

  // Get PRIMARY KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
        separator = this->GetListOf(StatementType::identifier);

        SyntaxAnalyzer::MakeKinship(node, separator);
      }
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<Node> node, column_name, separator, reference;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::foreignKey);

  // Get FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
        separator = this->GetListOf(StatementType::identifier);

        SyntaxAnalyzer::MakeKinship(node, separator);
      }
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  // Get Reference
  if (tokens_array_.empty()) {
    LOG(ERROR, "reference is missed");
    return node;
  } else {
    reference = this->GetReference();

    SyntaxAnalyzer::MakeKinship(node, reference);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetReference() {
  std::shared_ptr<Node> reference, ref_table_name;

  // Get REFERENCES
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> ref_key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  std::string ref_key_word = ref_key_word_node->get_data();

  if (ref_key_word != "REFERENCES") {
    LOG(ERROR, "incorrect reference");
    return reference;
  }

  reference = get_first_token();
  reference->set_st_type(StatementType::reference);

  // Get referenced table or columns
  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
    return reference;
  } else {
    ref_table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(reference, ref_table_name);
  }

  // Get columns if present
  if (!tokens_array_.empty()) {
    std::shared_ptr<Node> ref_column_name, next_ref_column_names;

    if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    }

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid foreign key: incorrect list of columnNames");
      return reference;
    } else {
      ref_column_name = this->GetName();

      SyntaxAnalyzer::MakeKinship(reference, ref_column_name);

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid foreign key: incorrect list of columnNames");
        return reference;
      } else {
        if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
          next_ref_column_names =
              this->GetListOf(StatementType::identifier);

          SyntaxAnalyzer::MakeKinship(reference, next_ref_column_names);
        }
      }
    }
  }

  return reference;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetCondition() {
  std::shared_ptr<Node> node;
  node = this->GetORCondition();
  node->set_st_type(StatementType::condition);

  /* LogExpr        <-- OR
   * OR             <-- AND-condition ('OR' AND-condition)*
   * AND-condition  <-- NOT-condition ('AND' NOT-condition)*
   * NOT-condition  <-- Predicate ('NOT' NOT-condition)?
   * Predicate      <-- Expr / '(' LogExpr ')'                  */

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetORCondition() {
  std::shared_ptr<Node> AND_condition;

  // TODO: Get it recursively
  AND_condition = this->GetANDCondition();

  return AND_condition;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetANDCondition() {
  std::shared_ptr<Node> node, NOT_condition, AND_operator, next_NOT_conditions;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::ANDcondition);

  NOT_condition = this->GetNOTCondition();

  SyntaxAnalyzer::MakeKinship(node, NOT_condition);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
      std::shared_ptr<StringNode> tmp =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (tmp->get_data() == "AND") {
        AND_operator = this->get_first_token();

        SyntaxAnalyzer::MakeKinship(node, AND_operator);
      }

      // TODO: Get NOT-conditions recursively
//      if (tokens_array_.empty()) {
//        LOG(ERROR, "invalid AND-condition: missed "
//                   "NOT-condition after the \'AND\' operator");
//        return node;
//      } else {
//        next_NOT_conditions =
//      }
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetNOTCondition() {
  std::shared_ptr<Node> node, NOT_operator, predicate;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
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
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
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
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::expression);

  /* Word?      --> GetName
   * Operator?  --> GetUnaryOperator
   * Bracket?   --> (, GetExpression, )
   * else:      --> GetMathExpression   */

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
      std::shared_ptr<StringNode> str;
    }
  }

  // So, it is Math expression
  std::shared_ptr<Node> expression = this->GetMathExpression();

  SyntaxAnalyzer::MakeKinship(node, expression);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetString() {
  std::shared_ptr<Node> node;

  bool is_single_quote =
      SyntaxAnalyzer::IsSingleQuote(this->peek_first_token());

  this->pop_first_token();
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid expression: invalid string");
    return node;
  } else {
    std::shared_ptr<StringNode> str;
    while (SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
           || SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
      std::shared_ptr<Node> tmp = this->get_first_token();
      DataType tmp_type = tmp->get_type();
      std::string new_data = str->get_data() + " ";
      switch (tmp_type) {
        case DataType::INT_NUMBER:
          new_data += std::to_string(
              std::dynamic_pointer_cast<IntNumNode>(tmp)->get_data());
          break;
        case DataType::FLOAT_NUMBER:
          new_data += std::to_string(
              std::dynamic_pointer_cast<FloatNumNode>(tmp)->get_data());
          break;
        case DataType::WORD:
        case DataType::OPERATOR:
          new_data += std::dynamic_pointer_cast<StringNode>(tmp)->get_data();
          break;
        case DataType::BRACKET:
        case DataType::PUNCTUATION:
          new_data += std::to_string(
              std::dynamic_pointer_cast<CharNode>(tmp)->get_data());
          break;
        default:
          LOG(ERROR,
              "invalid string: incorrect type of token inside the string");
          return node;
      }
      str->set_data(new_data);
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid string: closing quote is missed");
    return node;
  } else {
    if (is_single_quote) {
      this->ValidateIsSingleQuote(this->peek_first_token());
    } else {
      this->ValidateIsDoubleQuote(this->peek_first_token());
    }
    this->pop_first_token();
  }

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

  return node;
}
//std::shared_ptr<Node> SyntaxAnalyzer::GetMathSum();
//std::shared_ptr<Node> SyntaxAnalyzer::GetMathProduct();
//std::shared_ptr<Node> SyntaxAnalyzer::GetMathPower();
//std::shared_ptr<Node> SyntaxAnalyzer::GetMathValue();
std::shared_ptr<Node> SyntaxAnalyzer::GetName() {
  std::shared_ptr<Node> node, identifier, next_identifiers;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  node->set_st_type(StatementType::name);

  this->ValidateIsWord(this->peek_first_token());
  identifier = this->get_first_token();
  identifier->set_st_type(StatementType::identifier);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();

      SyntaxAnalyzer::MakeKinship(node, next_identifiers);
    }
  }

  SyntaxAnalyzer::MakeKinship(node, identifier);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifier() {
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<Node> identifier = this->get_first_token();
  identifier->set_st_type(StatementType::identifier);

  return identifier;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifiers() {
  std::shared_ptr<Node> dot, identifier, next_identifiers;

  dot = this->get_first_token();
  dot->set_st_type(StatementType::delimiter_dot);

  if (tokens_array_.empty()) {
    LOG(ERROR, "Bad name, which ends on dot");
    return nullptr;
  } else {
    identifier = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(dot, identifier);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();

      SyntaxAnalyzer::MakeKinship(dot, next_identifiers);
    }
  }

  return dot;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetListOf(
    StatementType get_function_type) {
  std::shared_ptr<Node> argument, separator, next_separator;

  // Get separator (comma)
  separator = this->get_first_token();
  if (!SyntaxAnalyzer::IsComma(separator)) {
    LOG(ERROR, "separator for the listOf is not a comma");
    return separator;
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "argument is missed");
    return separator;
  } else {
    switch (get_function_type) {
      case StatementType::identifier:argument = this->GetIdentifier();
        break;
      case StatementType::columnDefinition:
        argument = this->GetColumnDefinition();
        break;
      case StatementType::tableConstraint:argument = this->GetTableConstraint();
        break;
      default:LOG(ERROR, "unknown statement type for the listOf");
        break;
    }

    SyntaxAnalyzer::MakeKinship(separator, argument);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      next_separator = GetListOf(get_function_type);

      SyntaxAnalyzer::MakeKinship(separator, next_separator);
    }
  }

  return separator;
}

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_array_.empty()) {
    LOG(ERROR, "token's array are empty");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token's array are not empty");
}
void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<Node> &node) const {
  if (node->get_type() != DataType::WORD) {
    LOG(ERROR, "token is not a word");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a word");
}
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOG(ERROR, "token is not a opening round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a opening round bracket");
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOG(ERROR, "token is not a closing round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a closing round bracket");
}
void SyntaxAnalyzer::ValidateIsSingleQuote(std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a single quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a single quote");
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a double quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a double quote");
}

bool SyntaxAnalyzer::IsBracket(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::BRACKET) {
    LOG(DEBUG, "token is not a bracket: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a bracket");
    return true;
  }
}
bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::PUNCTUATION) {
    LOG(DEBUG, "token does not apply to punctuation: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token applies to punctuation");
    return true;
  }
}
bool SyntaxAnalyzer::IsWord(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::WORD) {
    LOG(DEBUG, "token is not a word: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a word");
    return true;
  }
}
bool SyntaxAnalyzer::IsOperator(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::OPERATOR) {
    LOG(DEBUG, "token is not a operator: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a operator");
    return true;
  }
}

bool SyntaxAnalyzer::IsDot(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> dot =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>('.', DataType::PUNCTUATION));
  return Node::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::PUNCTUATION));
  return Node::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(std::shared_ptr<Node> &node) {
  // Opening Round Bracket
  std::shared_ptr<Node> ORB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              '(', DataType::BRACKET));
  return Node::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(std::shared_ptr<Node> &node) {
  // Closing Round Bracket
  std::shared_ptr<Node> CRB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              ')', DataType::BRACKET));
  return Node::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> single_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\'", DataType::PUNCTUATION));
  return Node::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> double_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\"", DataType::PUNCTUATION));
  return Node::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(std::shared_ptr<Node> &node) {
  if (node->get_type() == DataType::OPERATOR) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->get_data();
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(std::shared_ptr<Node> &node) {
  if (node->get_type() == DataType::OPERATOR) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->get_data();
    std::vector<std::string> bin_operators = {
        "=",  "=", "<>",
        "!=", ">", ">=",
        "!>", "<", "<=",
        "!<"
    };
    if (std::find(bin_operators.begin(),
                  bin_operators.end(),
                  data) != bin_operators.end()) {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsSemicolon(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> semicolon =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(';', DataType::PUNCTUATION));
  return Node::IsNodesEqual(semicolon, node);
}

void SyntaxAnalyzer::MakeKinship(std::shared_ptr<Node> &parent,
                                 std::shared_ptr<Node> &child) {
  parent->AddChild(child);
  child->set_parent(parent);
}

std::shared_ptr<Node>& SyntaxAnalyzer::peek_first_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node> &>(tokens_array_.front());
}
std::shared_ptr<Node>& SyntaxAnalyzer::peek_last_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node> &>(tokens_array_.back());
}
std::shared_ptr<Node> SyntaxAnalyzer::get_first_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<Node> node = tokens_array_.front();
  tokens_array_.pop_front();
  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::get_last_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<Node> node = tokens_array_.back();
  tokens_array_.pop_back();
  return node;
}
void SyntaxAnalyzer::pop_first_token() {
  this->ValidateNotEmpty();
  tokens_array_.pop_front();
}
void SyntaxAnalyzer::pop_last_token() {
  this->ValidateNotEmpty();
  tokens_array_.pop_back();
}
