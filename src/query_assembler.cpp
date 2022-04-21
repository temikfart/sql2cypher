#include "SCC/query_assembler.h"

void QueryAssembler::Translate(std::shared_ptr<Node> AST) {
  LOG(INFO, "Starting translation...");

  ast_ = std::move(AST);

  if (ast_ == nullptr) {
    LOG(TRACE, "empty AST, nothing to translate");
    return;
  }

  if (ast_->get_st_type() == StatementType::Program) {
    if (ast_->get_children_amount() > 0) {
      this->TranslateProgram(ast_);
    } else {
      LOG(TRACE, "root of AST does not have children");
      return;
    }
  } else {
    LOG(ERROR,
        "invalid AST: root should be with \'Program\' statement type");
    exit(EXIT_FAILURE);
  }

  LOG(INFO, "Translation is ended");
}

void QueryAssembler::TranslateProgram(std::shared_ptr<Node> node) {
  auto query = node->get_child(0);
  if (query->get_st_type() == StatementType::query) {
    this->TranslateQuery(query);
  } else {
    LOG(ERROR, "first child of program node is not a query");
    exit(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->get_st_type() == StatementType::delimiter_semicolon) {
      if (other_queries->get_children_amount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOG(ERROR, "invalid delimiter between queries");
      exit(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(TRACE, "empty query");
    return;
  }

  auto data_language = node->get_child(0);
  switch (data_language->get_st_type()) {
    case StatementType::ddlStatement:this->TranslateDDLStatement(data_language);
      break;
    case StatementType::dmlStatement:this->TranslateDMLStatement(data_language);
      break;
    default:LOG(ERROR, "unknown query data language");
      exit(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateDDLStatement(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(TRACE, "empty DDL query");
    return;
  }

  auto statement = node->get_child(0);
  switch (statement->get_st_type()) {
    case StatementType::createDatabaseStatement:
      this->TranslateCreateDatabase(statement);
      break;
    case StatementType::createTableStatement:
      this->TranslateCreateTable(statement);
      break;
    case StatementType::alterTableStatement:this->TranslateAlterTable(statement);
      break;
    case StatementType::dropDatabaseStatement:
      this->TranslateDropDatabase(statement);
      break;
    case StatementType::dropTableStatement:this->TranslateDropTable(statement);
      break;
    default:LOG(ERROR, "unknown DDL statement");
      exit(EXIT_FAILURE);
  }
}
void QueryAssembler::TranslateDMLStatement(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(TRACE, "empty DML query");
  }

  auto statement = node->get_child(0);
  switch (statement->get_st_type()) {
    case StatementType::insertStatement:this->TranslateInsert(statement);
      break;
    case StatementType::deleteStatement:this->TranslateDelete(statement);
      break;
    case StatementType::updateStatement:this->TranslateUpdate(statement);
      break;
    default:LOG(ERROR, "unknown DML statement");
      exit(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateCreateDatabase(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "database name is missed");
    exit(EXIT_FAILURE);
  }

  out_ << "CREATE DATABASE "
       << this->TranslateName(node->get_child(0))
       << ";\n" << std::endl;
}
void QueryAssembler::TranslateCreateTable(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "table name is missed");
    exit(EXIT_FAILURE);
  }

  std::string table_name = this->TranslateName(node->get_child(0));
  out_ << "CREATE (:" << table_name;

  if (node->get_children_amount() < 2) {
    LOG(ERROR, "table definition is missed");
    exit(EXIT_FAILURE);
  }
  auto table_definition = node->get_child(1);
  if (table_definition->get_children_amount() == 0) {
    LOG(ERROR, "invalid table: empty table definition");
    exit(EXIT_FAILURE);
  }

  // Get properties
  auto column_definition = table_definition->get_child(0);
  if (column_definition->get_st_type() != StatementType::columnDefinition) {
    LOG(ERROR, "table without columns should not be created");
    exit(EXIT_FAILURE);
  }

  out_ << " {";

  StdProperty first_property =
      this->TranslateColumnDefinition(column_definition);
  out_ << std::get<0>(first_property) << ": "
       << std::get<1>(first_property);

  if (table_definition->get_children_amount() > 1) {
    auto comma = table_definition->get_child(1);
    if (comma->get_st_type() != StatementType::delimiter_comma) {
      LOG(ERROR, "delimiter in the table definition is not a comma");
      exit(EXIT_FAILURE);
    }

    if (comma->get_child(0)->get_st_type()
        == StatementType::columnDefinition) {
      std::vector<StdProperty> other_props =
          this->TranslateListOfColumnDefinitions(comma);
      for (auto &i: other_props) {
        out_ << ", " << std::get<0>(i) << ": " << std::get<1>(i);
      }
    }
  }

  out_ << "});\n" << std::endl;

  // Get constraints
  if (table_definition->get_children_amount() > 1) {
    auto constraints = this->FindConstraint(table_definition->get_child(1));
    if (constraints != nullptr) {
      this->TranslateTableConstraint(constraints, table_name);
    }
  }
}
void QueryAssembler::TranslateAlterTable(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "table name is missed");
    exit(EXIT_FAILURE);
  }

  std::string table_name = this->TranslateName(node->get_child(0));

  if (node->get_children_amount() < 2) {
    LOG(ERROR, "alter table action is missed");
    exit(EXIT_FAILURE);
  }

  auto action_node = node->get_child(1);
  if (action_node->get_st_type() == StatementType::alterActionADD) {
    if (action_node->get_children_amount() < 1) {
      LOG(ERROR, "alter table ADD without content");
      exit(EXIT_FAILURE);
    }
    auto table_definition = action_node->get_child(0);

    if (table_definition->get_children_amount() < 1) {
      LOG(ERROR, "alter table ADD action without arguments");
      exit(EXIT_FAILURE);
    }
    auto first_argument = table_definition->get_child(0);

    bool is_constraint_first = true;

    // Get column definitions
    if (first_argument->get_st_type() == StatementType::columnDefinition) {
      is_constraint_first = false;

      out_ << "MATCH (n:" << table_name << ")\n";
      out_ << "SET ";

      StdProperty first_prop =
          this->TranslateColumnDefinition(first_argument);
      out_ << "n." << std::get<0>(first_prop)
           << " = " << std::get<1>(first_prop);

      if (table_definition->get_children_amount() > 1) {
        auto comma = table_definition->get_child(1);
        if (comma->get_st_type() != StatementType::delimiter_comma) {
          LOG(ERROR, "delimiter between column definitions is not a comma");
          exit(EXIT_FAILURE);
        }

        if (comma->get_child(0)->get_st_type()
            == StatementType::columnDefinition) {
          std::vector<StdProperty> other_props =
              this->TranslateListOfColumnDefinitions(comma);

          for (auto& i : other_props) {
            out_ << ", n." << std::get<0>(i)
                 << " = " << std::get<1>(i);
          }
        }
      }

      out_ << ";\n" << std::endl;
    }

    // Get table constraints
    if (is_constraint_first) {
      this->TranslateTableConstraint(table_definition, table_name);
    } else {
      auto constraints =
          this->FindConstraint(table_definition->get_child(1));
      if (constraints != nullptr) {
        this->TranslateTableConstraint(constraints, table_name);
      }
    }
  } else if (action_node->get_st_type() == StatementType::alterActionDROP) {
    if (action_node->get_children_amount() < 1) {
      LOG(ERROR, "alter table DROP without content");
      exit(EXIT_FAILURE);
    }
    auto drop_list_def = action_node->get_child(0);

    if (drop_list_def->get_children_amount() < 1) {
      LOG(ERROR, "alter table DROP without arguments");
    }
    auto object = drop_list_def->get_child(0);

    this->TranslateDropObject(object, table_name);

    if (drop_list_def->get_children_amount() > 1) {
      auto other_objects = drop_list_def->get_child(1);
      this->TranslateListOfDropObjects(other_objects, table_name);
    }
  }
}
void QueryAssembler::TranslateDropDatabase(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "database name is missed");
    return;
  }

  std::vector<std::string> db_names;
  db_names.push_back(this->TranslateName(node->get_child(0)));

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_db_names =
        this->GetListOfNames(node->get_child(1));
    db_names.insert(db_names.end(),
                    other_db_names.begin(),
                    other_db_names.end());
  }

  for (auto &i: db_names) {
    out_ << "DROP DATABASE " << i << ";\n" << std::endl;
  }
}
void QueryAssembler::TranslateDropTable(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "table name is missed");
    return;
  }

  std::vector<std::string> table_names;
  table_names.push_back(this->TranslateName(node->get_child(0)));

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_table_names =
        this->GetListOfNames(node->get_child(1));
    table_names.insert(table_names.end(),
                       other_table_names.begin(),
                       other_table_names.end());
  }

  for (auto &i: table_names) {
    out_ << "MATCH (x:" << i << ") DELETE x;\n" << std::endl;
  }
}

std::vector<StdProperty> QueryAssembler::TranslateListOfColumnDefinitions(
    std::shared_ptr<Node> node) {
  auto first = node->get_child(0);
  if (first->get_st_type() != StatementType::columnDefinition) {
    LOG(TRACE, "list of column definitions is ended");
    return {};
  }

  std::vector<StdProperty> list_of_cols;
  list_of_cols.push_back(this->TranslateColumnDefinition(first));
  if (node->get_children_amount() > 1) {
    auto comma = node->get_child(1);
    if ((comma->get_child(0))->get_st_type()
        == StatementType::columnDefinition) {
      std::vector<StdProperty> other_cols =
          this->TranslateListOfColumnDefinitions(node->get_child(1));

      list_of_cols.insert(list_of_cols.end(),
                          other_cols.begin(),
                          other_cols.end());
    }
  }

  return list_of_cols;
}
StdProperty QueryAssembler::TranslateColumnDefinition(
    std::shared_ptr<Node> node) {
  std::string column_name = this->TranslateIdentifier(node->get_child(0));

  auto datatype_node = node->get_child(1);
  StatementType datatype = datatype_node->get_st_type();

  std::string datatype_str;
  switch (datatype) {
    case StatementType::SQL_int:datatype_str = "0";
      break;
    case StatementType::SQL_float:datatype_str = "0.0";
      break;
    case StatementType::SQL_char:
    case StatementType::SQL_varchar:datatype_str = "\"\"";
      break;
    default:LOG(ERROR, "unknown SQL datatype");
      datatype_str = "null";
  }

  return std::tie(column_name, datatype_str);
}

void QueryAssembler::TranslateTableConstraint(std::shared_ptr<Node> node,
                                              std::string table_name) {
  int child_with_key = 0;
  std::string constraint_name;

  auto constraint_serv_node = node->get_child(0);
  if (constraint_serv_node->get_children_amount() > 1) {
    child_with_key++;
    auto constraint_kw = constraint_serv_node->get_child(0);
    constraint_name +=
        this->TranslateIdentifier(constraint_kw->get_child(0));
  } else {
    constraint_name = "constraint";
  }

  // Translate constraint
  auto key = constraint_serv_node->get_child(child_with_key);
  if (key->get_st_type() == StatementType::primaryKey) {
    this->TranslatePrimaryKey(key, constraint_name, table_name);
  } else if (key->get_st_type() == StatementType::foreignKey) {
    this->TranslateForeignKey(key, constraint_name, table_name);
  }

  if (node->get_children_amount() > 1) {
    this->TranslateTableConstraint(
        node->get_child(1),
        table_name);
  }
}
std::shared_ptr<Node> QueryAssembler::FindConstraint(
    std::shared_ptr<Node> node) {
  std::shared_ptr<Node> constraints = nullptr;

  auto left = node->get_child(0);
  if (left->get_st_type() == StatementType::tableConstraint) {
    constraints = node;
  } else {
    if (node->get_children_amount() > 1) {
      constraints = this->FindConstraint(node->get_child(1));
    }
  }

  return constraints;
}

void QueryAssembler::TranslateDropObject(
    std::shared_ptr<Node> node,
    std::string table_name) {
  StatementType drop_type = node->get_st_type();
  if (drop_type == StatementType::dropColumn) {
    std::vector<std::string> properties = this->GetListOfProperties(node);

    out_ << "MATCH (n:" << table_name << ")\n";
    out_ << "SET ";
    for (size_t i = 0; i < properties.size(); i++) {
      out_ << "n." << properties[i] << " = null";
      if (i + 1 != properties.size()) {
        out_ << ", ";
      }
    }
    out_ << ";\n" << std::endl;
  } else if (drop_type == StatementType::dropConstraint) {
    std::vector<std::string> constraints = this->GetListOfProperties(node);
    for (auto &i: constraints) {
      out_ << "DROP CONSTRAINT " << i;
      out_ << ";\n" << std::endl;
    }
  } else {
    LOG(ERROR, "unknown type for the drop object");
    return;
  }
}
void QueryAssembler::TranslateListOfDropObjects(
    std::shared_ptr<Node> node,
    std::string table_name) {
  auto object = node->get_child(0);
  this->TranslateDropObject(object, table_name);

  if (node->get_children_amount() > 1) {
    auto other_objects = node->get_child(1);
    this->TranslateListOfDropObjects(other_objects, table_name);
  }
}

void QueryAssembler::TranslateInsert(std::shared_ptr<Node> node) {}
void QueryAssembler::TranslateDelete(std::shared_ptr<Node> node) {}
void QueryAssembler::TranslateUpdate(std::shared_ptr<Node> node) {}

// Basic statements

void QueryAssembler::TranslatePrimaryKey(
    std::shared_ptr<Node> key,
    std::string &constraint_name,
    std::string &table_name) {
  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(key->get_child(0)));

  if (key->get_children_amount() > 1) {
    std::vector<std::string> other_prop =
        this->GetListOfProperties(key->get_child(1));
    properties.insert(properties.end(),
                      other_prop.begin(),
                      other_prop.end());
  }

  this->CreateUniqueNodePropertyConstraint(
      (constraint_name + "_" + std::to_string(constraint_counter++)),
      table_name,
      properties
  );
  for (auto &i: properties) {
    this->CreateNodePropertyExistenceConstraint(
        (constraint_name + "_" + std::to_string(constraint_counter++)),
        table_name,
        i
    );
  }
}
void QueryAssembler::TranslateForeignKey(
    std::shared_ptr<Node> key,
    std::string &constraint_name,
    std::string &table_name) {
  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(key->get_child(0)));

  int reference_child_num = 1;
  if (key->get_children_amount() > 2) {
    reference_child_num++;

    std::vector<std::string> other_prop =
        this->GetListOfProperties(key->get_child(1));
    properties.insert(properties.end(),
                      other_prop.begin(),
                      other_prop.end());
  }

  // Get reference
  auto reference = key->get_child(reference_child_num);
  auto table_name_node = reference->get_child(0);
  std::string ref_table_name = this->TranslateName(table_name_node);

  // Get ref columns if present
  std::vector<std::string> ref_columns;

  size_t table_ch_num = table_name_node->get_children_amount();
  if (table_ch_num > 1) {
    auto second = table_name_node->get_child(1);
    size_t prop_num = 1;
    if (second->get_st_type() == StatementType::delimiter_dot) {
      prop_num++;
    }
    if (table_ch_num > prop_num) {
      ref_columns.push_back(this->TranslateIdentifier(table_name_node->get_child(
          prop_num)));
      prop_num++;
      if (table_ch_num > prop_num) {
        std::vector<std::string> other_props =
            this->GetListOfProperties(table_name_node->get_child(prop_num));
        ref_columns.insert(ref_columns.end(),
                           other_props.begin(),
                           other_props.end());
      }
    }
  }

  // Create Node Property Existence Constraint
  // for the properties and ref columns
  for (auto &i: properties) {
    this->CreateNodePropertyExistenceConstraint(
        (constraint_name + "_" + std::to_string(constraint_counter++)),
        table_name,
        i
    );
  }
  for (auto &i: ref_columns) {
    this->CreateNodePropertyExistenceConstraint(
        (constraint_name + "_" + std::to_string(constraint_counter++)),
        ref_table_name,
        i
    );
  }

  // Create Unique Node Property Constraint
  // for the ref columns

  this->CreateUniqueNodePropertyConstraint(
      (constraint_name + std::to_string(constraint_counter++)),
      ref_table_name,
      ref_columns
  );
}
void QueryAssembler::CreateUniqueNodePropertyConstraint(
    const std::string &constraint_name,
    const std::string &LabelName,
    const std::vector<std::string> &properties) {
  out_ << "CREATE CONSTRAINT [" << constraint_name << "]" << std::endl;
  out_ << "FOR (n:" << LabelName << ")" << std::endl;
  out_ << "REQUIRE (";
  for (size_t i = 0; i < properties.size(); i++) {
    out_ << "n." << properties[i];
    if (i + 1 != properties.size()) {
      out_ << ", ";
    }
  }
  out_ << ") IS UNIQUE;\n" << std::endl;
}
void QueryAssembler::CreateNodePropertyExistenceConstraint(
    const std::string &constraint_name,
    const std::string &LabelName,
    const std::string &property) {
  out_ << "CREATE CONSTRAINT [" << constraint_name << "]" << std::endl;
  out_ << "FOR (n:" << LabelName << ")" << std::endl;
  out_ << "REQUIRE (n." << property << ") IS NOT NULL;\n" << std::endl;
}

std::vector<std::string> QueryAssembler::GetListOfProperties(
    std::shared_ptr<Node> node) {
  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(node->get_child(0)));

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_prop =
        this->GetListOfProperties(node->get_child(1));
    properties.insert(properties.end(),
                      other_prop.begin(),
                      other_prop.end());
  }

  return properties;
}
std::vector<std::string> QueryAssembler::GetListOfNames(
    std::shared_ptr<Node> node) {
  std::vector<std::string> names;
  names.push_back(this->TranslateName(node->get_child(0)));

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_names =
        this->GetListOfNames(node->get_child(1));
    names.insert(names.end(),
                 other_names.begin(),
                 other_names.end());
  }

  return names;
}

std::string QueryAssembler::TranslateName(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "empty name");
    return "";
  }

  std::ostringstream name;

  name << this->TranslateIdentifier(node->get_child(0));

  if (node->get_children_amount() > 1) {
    auto dot = node->get_child(1);
    if (dot->get_st_type() == StatementType::delimiter_dot) {
      name << this->TranslateIdentifiers(node->get_child(1));
    }
  }

  return name.str();
}
std::string QueryAssembler::TranslateIdentifiers(
    std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "invalid list of identifiers");
    return "";
  }

  std::ostringstream identifiers;
  identifiers << "." << this->TranslateIdentifier(node->get_child(0));

  if (node->get_children_amount() > 1) {
    identifiers << this->TranslateIdentifiers(node->get_child(1));
  }

  return identifiers.str();
}
std::string QueryAssembler::TranslateIdentifier(
    std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(ERROR, "empty identifier");
    return "";
  }

  auto string_node =
      std::dynamic_pointer_cast<StringNode>(node->get_child(0));

  return string_node->get_data();
}
