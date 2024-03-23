#include "SCC/query_assembler.h"

void QueryAssembler::TranslateDDLStatement(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGD << "empty DDL query";
    return;
  }

  auto statement = node->get_child(0);
  switch (statement->stmt_type) {
    case StmtType::kCreateDatabaseStmt:
      this->TranslateCreateDatabase(statement);
      break;
    case StmtType::kCreateTableStmt:
      this->TranslateCreateTable(statement);
      break;
    case StmtType::kAlterTableStmt:
      this->TranslateAlterTable(statement);
      break;
    case StmtType::kDropDatabaseStmt:
      this->TranslateDropDatabase(statement);
      break;
    case StmtType::kDropTableStmt:
      this->TranslateDropTable(statement);
      break;
    default:
      LOGE << "unknown DDL statement";
      end(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateCreateDatabase(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "database name is missed";
    end(EXIT_FAILURE);
  }

  out_ << "CREATE DATABASE "
       << this->TranslateName(node->get_child(0))
       << ";\n" << std::endl;
}
void QueryAssembler::TranslateCreateTable(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "table name is missed";
    end(EXIT_FAILURE);
  }

  std::string table_name = this->TranslateName(node->get_child(0));
  out_ << "CREATE (:" << table_name;

  if (node->get_children_amount() < 2) {
    LOGE << "table definition is missed";
    end(EXIT_FAILURE);
  }
  auto table_definition = node->get_child(1);
  if (table_definition->get_children_amount() == 0) {
    LOGE << "invalid table: empty table definition";
    end(EXIT_FAILURE);
  }

  // Get properties
  auto column_definition = table_definition->get_child(0);
  if (column_definition->stmt_type != StmtType::kColumnDef) {
    LOGE << "table without columns should not be created";
    end(EXIT_FAILURE);
  }

  out_ << " {";

  StdProperty first_property =
      this->TranslateColumnDefinition(column_definition);
  out_ << std::get<0>(first_property) << ": "
       << std::get<1>(first_property);

  if (table_definition->get_children_amount() > 1) {
    auto comma = table_definition->get_child(1);
    if (comma->stmt_type != StmtType::kCommaDelimiter) {
      LOGE << "invalid table definition: delimiter is not a comma";
      end(EXIT_FAILURE);
    }

    if (comma->get_children_amount() == 0) {
      LOGE << "invalid table definition: comma without children";
      end(EXIT_FAILURE);
    }
    if (comma->get_child(0)->stmt_type
        == StmtType::kColumnDef) {
      std::vector<StdProperty> other_props =
          this->TranslateListOfColumnDefinitions(comma);
      for (auto& i: other_props) {
        out_ << ", " << std::get<0>(i) << ": " << std::get<1>(i);
      }
    }
  }

  out_ << "});\n" << std::endl;

  // Get constraints
  if (table_definition->get_children_amount() > 1) {
    auto constraints = this->FindConstraint(table_definition->get_child(1));
    if (constraints != nullptr) {
      this->TranslateListOfTableConstraints(constraints, table_name);
    }
  }
}
void QueryAssembler::TranslateAlterTable(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "table name is missed";
    end(EXIT_FAILURE);
  }

  std::string table_name = this->TranslateName(node->get_child(0));

  if (node->get_children_amount() < 2) {
    LOGE << "alter table action is missed";
    end(EXIT_FAILURE);
  }

  auto action_node = node->get_child(1);
  if (action_node->stmt_type == StmtType::kAlterActionAdd) {
    this->TranslateAlterTableActionAdd(action_node, table_name);
  } else if (action_node->stmt_type == StmtType::kAlterActionDrop) {
    this->TranslateAlterTableActionDrop(action_node, table_name);
  }
}
void QueryAssembler::TranslateDropDatabase(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "database name is missed";
    end(EXIT_FAILURE);
  }

  out_ << "DROP DATABASE "
       << this->TranslateName(node->get_child(0))
       << ";\n" << std::endl;

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_db_names =
        this->GetListOf(node->get_child(1), StmtType::kName);
    for (auto& i: other_db_names) {
      out_ << "DROP DATABASE " << i << ";\n" << std::endl;
    }
  }
}
void QueryAssembler::TranslateDropTable(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "table name is missed";
    end(EXIT_FAILURE);
  }

  std::vector<std::string> table_names;
  out_ << "MATCH (x:"
       << this->TranslateName(node->get_child(0))
       << ") DELETE x;\n" << std::endl;

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_table_names =
        this->GetListOf(node->get_child(1), StmtType::kName);
    for (auto& i: other_table_names) {
      out_ << "MATCH (x:" << i << ") DELETE x;\n" << std::endl;
    }
  }
}

void QueryAssembler::TranslateAlterTableActionAdd(
    std::shared_ptr<INode> action_node,
    std::string& table_name) {
  if (action_node->get_children_amount() == 0) {
    LOGE << "alter table ADD without content";
    end(EXIT_FAILURE);
  }
  auto table_definition = action_node->get_child(0);

  if (table_definition->get_children_amount() == 0) {
    LOGE << "alter table ADD action without arguments";
    end(EXIT_FAILURE);
  }
  auto first_argument = table_definition->get_child(0);

  bool is_constraint_first = true;

  // Get column definitions
  if (first_argument->stmt_type == StmtType::kColumnDef) {
    is_constraint_first = false;

    out_ << "MATCH (n:" << table_name << ")\n";
    out_ << "SET ";

    StdProperty first_prop =
        this->TranslateColumnDefinition(first_argument);
    out_ << "n." << std::get<0>(first_prop)
         << " = " << std::get<1>(first_prop);

    if (table_definition->get_children_amount() > 1) {
      auto comma = table_definition->get_child(1);
      if (comma->stmt_type != StmtType::kCommaDelimiter) {
        LOGE << "delimiter between column definitions is not a comma";
        end(EXIT_FAILURE);
      }

      if (comma->get_child(0)->stmt_type
          == StmtType::kColumnDef) {
        std::vector<StdProperty> other_props =
            this->TranslateListOfColumnDefinitions(comma);

        for (auto& i: other_props) {
          out_ << ", n." << std::get<0>(i)
               << " = " << std::get<1>(i);
        }
      }
    }

    out_ << ";\n" << std::endl;
  }

  // Get table constraints
  if (is_constraint_first) {
    this->TranslateListOfTableConstraints(table_definition, table_name);
  } else {
    auto constraints =
        this->FindConstraint(table_definition->get_child(1));
    if (constraints != nullptr) {
      this->TranslateListOfTableConstraints(constraints, table_name);
    }
  }
}
void QueryAssembler::TranslateAlterTableActionDrop(
    std::shared_ptr<INode> action_node,
    std::string& table_name) {
  if (action_node->get_children_amount() == 0) {
    LOGE << "alter table DROP without content";
    end(EXIT_FAILURE);
  }
  auto drop_list_def = action_node->get_child(0);

  if (drop_list_def->get_children_amount() == 0) {
    LOGE << "alter table DROP without arguments";
  }
  auto object = drop_list_def->get_child(0);

  this->TranslateDropObject(object, table_name);

  if (drop_list_def->get_children_amount() > 1) {
    auto other_objects = drop_list_def->get_child(1);
    this->TranslateListOfDropObjects(other_objects, table_name);
  }
}

std::vector<StdProperty> QueryAssembler::TranslateListOfColumnDefinitions(
    std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "invalid list of column definitions: comma without children";
    end(EXIT_FAILURE);
  }
  auto argument = node->get_child(0);
  if (argument->stmt_type != StmtType::kColumnDef) {
    return {};
  }

  std::vector<StdProperty> column_definitions;
  column_definitions.push_back(this->TranslateColumnDefinition(argument));
  if (node->get_children_amount() > 1) {
    auto comma = node->get_child(1);
    if ((comma->get_child(0))->stmt_type
        == StmtType::kColumnDef) {
      std::vector<StdProperty> other_cols =
          this->TranslateListOfColumnDefinitions(node->get_child(1));
      column_definitions.insert(column_definitions.end(),
                                other_cols.begin(),
                                other_cols.end());
    }
  }

  return column_definitions;
}
StdProperty QueryAssembler::TranslateColumnDefinition(
    std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "empty column definition";
    end(EXIT_FAILURE);
  }
  std::string column_name = this->TranslateIdentifier(node->get_child(0));

  if (node->get_children_amount() == 1) {
    LOGE << "invalid column definition: datatype is missed";
    end(EXIT_FAILURE);
  }
  StmtType datatype = node->get_child(1)->stmt_type;

  std::string datatype_str;
  switch (datatype) {
    case StmtType::kIntType:
      datatype_str = "0";
      break;
    case StmtType::kFloatType:
      datatype_str = "0.0";
      break;
    case StmtType::kCharType:
    case StmtType::kVarcharType:
      datatype_str = "\"\"";
      break;
    default:
      LOGE << "unknown SQL datatype";
      end(EXIT_FAILURE);
  }

  return std::tie(column_name, datatype_str);
}

void QueryAssembler::TranslateListOfTableConstraints(std::shared_ptr<INode> node,
                                                     std::string& table_name) {
  if (node->get_children_amount() == 0) {
    LOGE <<
        "invalid list of table constraints: comma without children";
    end(EXIT_FAILURE);
  }

  auto constraint = node->get_child(0);
  if (constraint->get_children_amount() == 0) {
    LOGE << "empty table constraint";
    end(EXIT_FAILURE);
  }

  // Create new constraint name if necessary
  size_t key_node_number = 0;
  std::string constraint_name;
  if (constraint->get_children_amount() > 1) {
    key_node_number++;

    auto constraint_kw = constraint->get_child(0);
    if (constraint_kw->stmt_type != StmtType::kConstraintKW
        || constraint_kw->get_children_amount() == 0) {
      LOGE << "invalid CONSTRAINT key word node";
      end(EXIT_FAILURE);
    }

    constraint_name +=
        this->TranslateIdentifier(constraint_kw->get_child(0));
  } else {
    constraint_name = table_name + "_constraint";
  }

  // Translate constraint definition
  if (constraint->get_children_amount() < key_node_number) {
    LOGE << "table constraint definition is missed";
    end(EXIT_FAILURE);
  }
  auto key = constraint->get_child(key_node_number);
  if (key->stmt_type == StmtType::kPrimaryKey) {
    this->TranslatePrimaryKey(key, constraint_name, table_name);
  } else if (key->stmt_type == StmtType::kForeignKey) {
    this->TranslateForeignKey(key, table_name);
  }

  if (node->get_children_amount() > 1) {
    this->TranslateListOfTableConstraints(
        node->get_child(1),
        table_name);
  }
}
std::shared_ptr<INode> QueryAssembler::FindConstraint(
    std::shared_ptr<INode> node) {
  std::shared_ptr<INode> constraints = nullptr;
  if (node->get_children_amount() == 0) {
    return constraints;
  }

  auto left = node->get_child(0);
  if (left->stmt_type == StmtType::kTableConstraint) {
    constraints = node;
  } else {
    if (node->get_children_amount() > 1) {
      constraints = this->FindConstraint(node->get_child(1));
    }
  }

  return constraints;
}

void QueryAssembler::TranslateListOfDropObjects(
    std::shared_ptr<INode> node,
    std::string& table_name) {
  if (node->stmt_type != StmtType::kDropList) {
    LOGE << "invalid statement type for the dropList";
    end(EXIT_FAILURE);
  }
  if (node->get_children_amount() == 0) {
    LOGE << "invalid dropList without children";
    end(EXIT_FAILURE);
  }

  this->TranslateDropObject(node->get_child(0), table_name);

  if (node->get_children_amount() > 1) {
    auto other_objects = node->get_child(1);
    this->TranslateListOfDropObjects(other_objects, table_name);
  }
}
void QueryAssembler::TranslateDropObject(
    std::shared_ptr<INode> node,
    std::string& table_name) {
  if (node->get_children_amount() == 0) {
    LOGE << "invalid drop object without children";
    end(EXIT_FAILURE);
  }
  std::string argument = this->TranslateIdentifier(node->get_child(0));
  std::vector<std::string> other_arguments;
  if (node->get_children_amount() > 1) {
    other_arguments =
        this->GetListOf(node->get_child(1), StmtType::kIdentifier);
  }

  if (node->stmt_type == StmtType::kDropColumn) {
    out_ << "MATCH (n:" << table_name << ")\n";
    out_ << "SET n." << argument << " = null";
    for (auto& i: other_arguments) {
      out_ << ", n." << i << " = null";
    }
    out_ << ";\n" << std::endl;
  } else if (node->stmt_type == StmtType::kDropConstraint) {
    out_ << "DROP CONSTRAINT " << argument << ";\n" << std::endl;
    for (auto& i: other_arguments) {
      out_ << "DROP CONSTRAINT " << i;
      out_ << ";\n" << std::endl;
    }
  } else {
    LOGE << "unknown type of the drop object";
    end(EXIT_FAILURE);
  }
}
