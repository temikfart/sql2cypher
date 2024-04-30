#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslateDDLStatement(const NodePtr<INode>& ddl_statement) {
  switch (ddl_statement->stmt_type) {
    case StmtType::kCreateDatabaseStmt:
      TranslateCreateDatabaseStatement(ddl_statement);
      break;
    case StmtType::kCreateTableStmt:
      TranslateCreateTableStatement(ddl_statement);
      break;
    case StmtType::kAlterTableStmt:
      TranslateAlterTableStatement(ddl_statement);
      break;
    case StmtType::kDropDatabaseStmt:
      TranslateDropDatabaseStatement(ddl_statement);
      break;
    case StmtType::kDropTableStmt:
      TranslateDropTableStatement(ddl_statement);
      break;
    default:
      throw translation_error("Unknown DDL statement type: \'"
                                  + ddl_statement->stmt_type.ToString() + "\'");
  }
}

void Translator::TranslateCreateDatabaseStatement(const NodePtr<INode>& stmt) {
  out_ << "CREATE DATABASE "
       << TranslateName(stmt->get_child(0))
       << ";\n" << std::endl;
}
void Translator::TranslateCreateTableStatement(const NodePtr<INode>& stmt) {
  const std::string msg_suffix = " in \'CREATE TABLE\' statement";

  auto table_name_node = stmt->get_child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = TranslateName(table_name_node);
  out_ << "CREATE (:" << table_name;

  ValidateHasChildren(stmt, 2, "Missing table definition" + msg_suffix);
  auto table_definition = stmt->get_child(1);

  ValidateHasChildren(table_definition, 1, "Empty table definition" + msg_suffix);
  auto column_definition = table_definition->get_child(0);

  ValidateIsCorrectStmtType(column_definition, StmtType::kColumnDef);
  out_ << " {";
  StdProperty first_property = TranslateColumnDefinition(column_definition);
  out_ << std::get<0>(first_property) << ": " << std::get<1>(first_property);

  if (HasChildren(table_definition, 2)) {
    auto comma = table_definition->get_child(1);
    ValidateIsCorrectStmtType(comma, StmtType::kCommaDelimiter);
    ValidateHasChildren(comma, 1, "Invalid table definition" + msg_suffix
        + ": comma without next element");

    if (IsCorrectStmtType(comma->get_child(0), StmtType::kColumnDef)) {
      std::vector<StdProperty> other_props = TranslateColumnDefinitions(comma);
      for (auto& i: other_props) {
        out_ << ", " << std::get<0>(i) << ": " << std::get<1>(i);
      }
    }
  }
  out_ << "});\n" << std::endl;

  if (HasChildren(table_definition, 2)) {
    auto constraints = FindConstraint(table_definition->get_child(1));
    if (constraints != nullptr) {
      TranslateListOfTableConstraints(constraints, table_name);
    }
  }
}
void Translator::TranslateAlterTableStatement(const NodePtr<INode>& stmt) {
  const std::string msg_suffix = " in \'ALTER TABLE\' statement";

  auto table_name_node = stmt->get_child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = TranslateName(table_name_node);

  ValidateHasChildren(stmt, 2, "Missing action" + msg_suffix);
  auto action_node = stmt->get_child(1);
  ValidateHasChildren(action_node);
  switch (action_node->stmt_type) {
    case StmtType::kAddKW:
      TranslateAlterTableActionAdd(action_node, table_name);
      break;
    case StmtType::kDropKW:
      TranslateAlterTableActionDrop(action_node, table_name);
      break;
    default:
      throw translation_error("Unknown action type \'" + action_node->stmt_type.ToString()
                                  + "\'" + msg_suffix);
  }
}
void Translator::TranslateDropDatabaseStatement(const NodePtr<INode>& stmt) {
  auto database_name_node = stmt->get_child(0);
  ValidateHasChildren(database_name_node);
  out_ << "DROP DATABASE " << TranslateName(database_name_node) << ";\n" << std::endl;

  if (HasChildren(stmt, 2)) {
    std::vector<std::string> other_db_names = GetListOf(stmt->get_child(1), StmtType::kName);
    for (auto& i: other_db_names) {
      out_ << "DROP DATABASE " << i << ";\n" << std::endl;
    }
  }
}
void Translator::TranslateDropTableStatement(const NodePtr<INode>& stmt) {
  auto table_name_node = stmt->get_child(0);
  ValidateHasChildren(table_name_node);
  out_ << "MATCH (x:" << TranslateName(table_name_node) << ") DELETE x;\n" << std::endl;

  if (HasChildren(stmt, 2)) {
    std::vector<std::string> other_table_names = GetListOf(stmt->get_child(1), StmtType::kName);
    for (auto& i: other_table_names) {
      out_ << "MATCH (x:" << i << ") DELETE x;\n" << std::endl;
    }
  }
}

void Translator::TranslateAlterTableActionAdd(const NodePtr<INode>& action_node,
                                              std::string& table_name) {
  const std::string msg_suffix = " in \'ALTER TABLE ADD\' statement";

  auto table_definition = action_node->get_child(0);

  ValidateHasChildren(table_definition, 1, "Missing column definition or constraint" + msg_suffix);
  auto first_argument = table_definition->get_child(0);

  if (IsCorrectStmtType(first_argument, StmtType::kColumnDef)) {
    out_ << "MATCH (n:" << table_name << ")\n";
    out_ << "SET ";

    StdProperty first_prop = TranslateColumnDefinition(first_argument);
    out_ << "n." << std::get<0>(first_prop) << " = " << std::get<1>(first_prop);

    if (HasChildren(table_definition, 2)) {
      auto comma = table_definition->get_child(1);
      ValidateIsCorrectStmtType(comma, StmtType::kCommaDelimiter);

      if (IsCorrectStmtType(comma->get_child(0), StmtType::kColumnDef)) {
        std::vector<StdProperty> other_props = TranslateColumnDefinitions(comma);
        for (auto& i: other_props) {
          out_ << ", n." << std::get<0>(i) << " = " << std::get<1>(i);
        }
      }
    }

    out_ << ";\n" << std::endl;

    auto constraints = FindConstraint(table_definition->get_child(1));
    if (constraints != nullptr) {
      TranslateListOfTableConstraints(constraints, table_name);
    }
  } else {
    TranslateListOfTableConstraints(table_definition, table_name);
  }
}
void Translator::TranslateAlterTableActionDrop(const NodePtr<INode>& action_node,
                                               std::string& table_name) {
  std::string msg_suffix = " in \'ALTER TABLE DROP\' statement";

  auto drop_list_def = action_node->get_child(0);

  ValidateHasChildren(drop_list_def, 1, "Missing arguments" + msg_suffix);
  auto object = drop_list_def->get_child(0);
  ValidateHasChildren(object);
  TranslateDropElement(object, table_name);

  if (HasChildren(drop_list_def, 2)) {
    auto other_objects = drop_list_def->get_child(1);
    ValidateHasChildren(other_objects);
    TranslateDropElements(other_objects, table_name);
  }
}

std::vector<StdProperty> Translator::TranslateColumnDefinitions(
    const NodePtr<INode>& column_definition) {
  auto argument = column_definition->get_child(0);
  ValidateIsCorrectStmtType(argument, StmtType::kColumnDef);

  ValidateHasChildren(argument);
  std::vector<StdProperty> column_definitions;
  column_definitions.push_back(TranslateColumnDefinition(argument));
  if (HasChildren(column_definition, 2)) {
    auto comma = column_definition->get_child(1);
    if (IsCorrectStmtType(comma->get_child(0), StmtType::kColumnDef)) {
      std::vector<StdProperty> other_cols = TranslateColumnDefinitions(comma);
      column_definitions.insert(column_definitions.end(), other_cols.begin(), other_cols.end());
    }
  }

  return column_definitions;
}
StdProperty Translator::TranslateColumnDefinition(const NodePtr<INode>& node) {
  auto column_name_node = node->get_child(0);
  ValidateHasChildren(column_name_node);
  std::string column_name = TranslateIdentifier(column_name_node);

  ValidateHasChildren(node, 2, "Missing datatype in column definition");
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
      throw translation_error("Unknown SQL datatype \'" + datatype.ToString() + "\'");
  }

  return std::tie(column_name, datatype_str);
}

void Translator::TranslateListOfTableConstraints(const NodePtr<INode>& node,
                                                 std::string& table_name) {
  auto constraint = node->get_child(0);
  ValidateHasChildren(constraint, 1, "Missing constraint definition");

  size_t key_node_number = 0;
  std::string constraint_name;
  if (HasChildren(constraint, 2)) {
    key_node_number++;

    auto constraint_kw = constraint->get_child(0);
    ValidateIsCorrectStmtType(constraint_kw, StmtType::kConstraintKW);
    ValidateHasChildren(constraint_kw);
    constraint_name += TranslateIdentifier(constraint_kw->get_child(0));
  } else {
    constraint_name = table_name + "_constraint";
  }

  ValidateHasChildren(constraint, key_node_number, "Missing constraint definition");
  auto key = constraint->get_child(key_node_number);
  ValidateHasChildren(key);
  switch (key->stmt_type) {
    case StmtType::kPrimaryKey:
      TranslatePrimaryKey(key, constraint_name, table_name);
      break;
    case StmtType::kForeignKey:
      TranslateForeignKey(key, table_name);
      break;
    default:
      throw translation_error("Unknown constraint type \'" + key->stmt_type.ToString() + "\'");
  }

  if (HasChildren(node, 2)) {
    TranslateListOfTableConstraints(node->get_child(1), table_name);
  }
}
std::shared_ptr<INode> Translator::FindConstraint(const NodePtr<INode>& node) {
  std::shared_ptr<INode> constraints = nullptr;
  if (!HasChildren(node)) {
    return constraints;
  }

  auto left = node->get_child(0);
  if (IsCorrectStmtType(left, StmtType::kTableConstraint)) {
    constraints = node;
  } else {
    if (HasChildren(node, 2)) {
      constraints = FindConstraint(node->get_child(1));
    }
  }

  return constraints;
}

void Translator::TranslateDropElements(const NodePtr<INode>& node, std::string& table_name) {
  ValidateIsCorrectStmtType(node, StmtType::kDropList);
  auto drop_object = node->get_child(0);
  ValidateHasChildren(drop_object);
  TranslateDropElement(drop_object, table_name);

  if (HasChildren(node, 2)) {
    auto other_objects = node->get_child(1);
    TranslateDropElements(other_objects, table_name);
  }
}
void Translator::TranslateDropElement(const NodePtr<INode>& node, std::string& table_name) {
  std::string argument = TranslateIdentifier(node->get_child(0));
  std::vector<std::string> other_arguments;
  if (HasChildren(node, 2)) {
    other_arguments = GetListOf(node->get_child(1), StmtType::kIdentifier);
  }

  switch (node->stmt_type) {
    case StmtType::kDropColumn:
      out_ << "MATCH (n:" << table_name << ")\n";
      out_ << "SET n." << argument << " = null";
      for (auto& i: other_arguments) {
        out_ << ", n." << i << " = null";
      }
      out_ << ";\n" << std::endl;
      break;
    case StmtType::kDropConstraint:
      out_ << "DROP CONSTRAINT " << argument << ";\n" << std::endl;
      for (auto& i: other_arguments) {
        out_ << "DROP CONSTRAINT " << i;
        out_ << ";\n" << std::endl;
      }
      break;
    default:
      throw translation_error("Unknown drop object type \'" + node->stmt_type.ToString() + "\'");
  }
}

} // scc::translator
