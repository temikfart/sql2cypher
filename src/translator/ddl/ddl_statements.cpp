#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace cypher;

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
  auto database_name_node = stmt->Child(0);
  ValidateHasChildren(database_name_node);
  std::string database_name = GetName(database_name_node);

  WriteCypherQuery(CreateDatabaseClauseBuilder::Build(database_name));
}
void Translator::TranslateCreateTableStatement(const NodePtr<INode>& stmt) {
  const std::string msg_suffix = " in \'CREATE TABLE\' statement";

  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = GetName(table_name_node);
  Node node(table_name);

  ValidateHasChildren(stmt, 2, "Missing table definition" + msg_suffix);
  auto table_definition = stmt->Child(1);

  ValidateHasChildren(table_definition, 1, "Empty table definition" + msg_suffix);
  auto column_definition = table_definition->Child(0);

  ValidateIsCorrectStmtType(column_definition, StmtType::kColumnDef);
  NodeProperty first_property = TranslateColumnDefinition(column_definition);
  node.AddProperty(first_property);

  if (HasChildren(table_definition, 2)) {
    auto comma = table_definition->Child(1);
    ValidateIsCorrectStmtType(comma, StmtType::kCommaDelimiter);
    ValidateHasChildren(comma, 1, "Invalid table definition" + msg_suffix
        + ": comma without next element");

    if (IsCorrectStmtType(comma->Child(0), StmtType::kColumnDef)) {
      std::vector<NodeProperty> other_props = TranslateColumnDefinitions(comma);
      for (auto& prop: other_props) {
        node.AddProperty(prop);
      }
    }
  }

  WriteCypherQuery(CreateNodeClauseBuilder::Build(node));

  if (HasChildren(table_definition, 2)) {
    auto constraints = FindConstraint(table_definition->Child(1));
    if (constraints != nullptr) {
      TranslateListOfTableConstraints(constraints, table_name);
    }
  }
}
void Translator::TranslateAlterTableStatement(const NodePtr<INode>& stmt) {
  const std::string msg_suffix = " in \'ALTER TABLE\' statement";

  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = GetName(table_name_node);

  ValidateHasChildren(stmt, 2, "Missing action" + msg_suffix);
  auto action_node = stmt->Child(1);
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
  auto database_name_node = stmt->Child(0);
  ValidateHasChildren(database_name_node);
  auto database_name = GetName(database_name_node);
  WriteCypherQuery(DropDatabaseClauseBuilder::Build(database_name));

  if (HasChildren(stmt, 2)) {
    std::vector<std::string> other_db_names = GetList(stmt->Child(1), StmtType::kName);
    for (const auto& other_database_name: other_db_names) {
      WriteCypherQuery(DropDatabaseClauseBuilder::Build(other_database_name));
    }
  }
}
void Translator::TranslateDropTableStatement(const NodePtr<INode>& stmt) {
  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);

  std::string table_name = GetName(table_name_node);
  Node node(table_name);
  WriteCypherQuery(DeleteNodeClauseBuilder::Build(node));

  if (HasChildren(stmt, 2)) {
    std::vector<std::string> other_table_names = GetList(stmt->Child(1), StmtType::kName);
    for (auto& other_table_name: other_table_names) {
      Node other_node(other_table_name);
      WriteCypherQuery(DeleteNodeClauseBuilder::Build(other_node));
    }
  }
}

void Translator::TranslateAlterTableActionAdd(const NodePtr<INode>& action_node,
                                              std::string& table_name) {
  const std::string msg_suffix = " in \'ALTER TABLE ADD\' statement";

  auto table_definition = action_node->Child(0);

  ValidateHasChildren(table_definition, 1, "Missing column definition or constraint" + msg_suffix);
  auto first_argument = table_definition->Child(0);

  if (IsCorrectStmtType(first_argument, StmtType::kColumnDef)) {
    Node node(table_name, "n");

    NodeProperty first_prop = TranslateColumnDefinition(first_argument);
    WriteCypherQuery(SetPropertyClauseBuilder::Build(node, first_prop));

    if (HasChildren(table_definition, 2)) {
      auto comma = table_definition->Child(1);
      ValidateIsCorrectStmtType(comma, StmtType::kCommaDelimiter);

      if (IsCorrectStmtType(comma->Child(0), StmtType::kColumnDef)) {
        std::vector<NodeProperty> other_props = TranslateColumnDefinitions(comma);
        for (auto& prop: other_props) {
          WriteCypherQuery(SetPropertyClauseBuilder::Build(node, prop));
        }
      }
    }

    auto constraints = FindConstraint(table_definition->Child(1));
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

  auto drop_list_def = action_node->Child(0);

  ValidateHasChildren(drop_list_def, 1, "Missing arguments" + msg_suffix);
  auto object = drop_list_def->Child(0);
  ValidateHasChildren(object);
  TranslateDropElement(object, table_name);

  if (HasChildren(drop_list_def, 2)) {
    auto other_objects = drop_list_def->Child(1);
    ValidateHasChildren(other_objects);
    TranslateDropElements(other_objects, table_name);
  }
}

std::vector<NodeProperty> Translator::TranslateColumnDefinitions(
    const NodePtr<INode>& column_definition) {
  auto argument = column_definition->Child(0);
  ValidateIsCorrectStmtType(argument, StmtType::kColumnDef);

  ValidateHasChildren(argument);
  std::vector<NodeProperty> column_definitions;
  column_definitions.push_back(TranslateColumnDefinition(argument));
  if (HasChildren(column_definition, 2)) {
    auto comma = column_definition->Child(1);
    if (IsCorrectStmtType(comma->Child(0), StmtType::kColumnDef)) {
      std::vector<NodeProperty> other_cols = TranslateColumnDefinitions(comma);
      column_definitions.insert(column_definitions.end(), other_cols.begin(), other_cols.end());
    }
  }

  return column_definitions;
}
NodeProperty Translator::TranslateColumnDefinition(const NodePtr<INode>& node) {
  auto column_name_node = node->Child(0);
  ValidateHasChildren(column_name_node);
  std::string column_name = GetIdentifier(column_name_node);

  ValidateHasChildren(node, 2, "Missing datatype in column definition");
  StmtType datatype = node->Child(1)->stmt_type;

  std::string datatype_str;
  PropertyType property_type;
  switch (datatype) {
    case StmtType::kIntType:
      datatype_str = "0";
      property_type = PropertyType::kInteger;
      break;
    case StmtType::kFloatType:
      datatype_str = "0.0";
      property_type = PropertyType::kFloat;
      break;
    case StmtType::kCharType:
    case StmtType::kVarcharType:
      datatype_str = "\"\"";
      property_type = PropertyType::kString;
      break;
    default:
      throw translation_error("Unknown SQL datatype \'" + datatype.ToString() + "\'");
  }

  return NodeProperty(column_name, datatype_str, property_type);
}

void Translator::TranslateListOfTableConstraints(const NodePtr<INode>& node,
                                                 std::string& table_name) {
  auto constraint = node->Child(0);
  ValidateHasChildren(constraint, 1, "Missing constraint definition");

  size_t key_node_number = 0;
  std::string constraint_name;
  if (HasChildren(constraint, 2)) {
    key_node_number++;

    auto constraint_kw = constraint->Child(0);
    ValidateIsCorrectStmtType(constraint_kw, StmtType::kConstraintKW);
    ValidateHasChildren(constraint_kw);
    constraint_name += GetIdentifier(constraint_kw->Child(0));
  } else {
    constraint_name = table_name + "_constraint";
  }

  ValidateHasChildren(constraint, key_node_number, "Missing constraint definition");
  auto key = constraint->Child(key_node_number);
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
    TranslateListOfTableConstraints(node->Child(1), table_name);
  }
}
std::shared_ptr<INode> Translator::FindConstraint(const NodePtr<INode>& node) {
  std::shared_ptr<INode> constraints = nullptr;
  if (!HasChildren(node)) {
    return constraints;
  }

  auto left = node->Child(0);
  if (IsCorrectStmtType(left, StmtType::kTableConstraint)) {
    constraints = node;
  } else {
    if (HasChildren(node, 2)) {
      constraints = FindConstraint(node->Child(1));
    }
  }

  return constraints;
}

void Translator::TranslateDropElements(const NodePtr<INode>& node, std::string& table_name) {
  ValidateIsCorrectStmtType(node, StmtType::kDropList);
  for (unsigned i = 0; HasChildren(node, i + 1); ++i) {
    auto drop_object = node->Child(0);
    ValidateHasChildren(drop_object);
    TranslateDropElement(drop_object, table_name);
  }
}
void Translator::TranslateDropElement(const NodePtr<INode>& node, std::string& table_name) {
  std::string argument = GetName(node->Child(0));
  std::vector<std::string> other_arguments;
  if (HasChildren(node, 2)) {
    other_arguments = GetList(node->Child(1), StmtType::kName);
  }

  switch (node->stmt_type) {
    case StmtType::kDropColumn:
      RemoveProperty(table_name, argument);
      for (const auto& arg: other_arguments) {
        RemoveProperty(table_name, arg);
      }
      break;
    case StmtType::kDropConstraint:
      WriteCypherQuery(DropConstraintClauseBuilder::Build(argument));
      for (const auto& arg: other_arguments) {
        WriteCypherQuery(DropConstraintClauseBuilder::Build(arg));
      }
      break;
    default:
      throw translation_error("Unknown drop object type \'" + node->stmt_type.ToString() + "\'");
  }
}

} // scc::translator
