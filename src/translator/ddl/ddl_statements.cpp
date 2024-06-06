#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace schema;

using cypher::PropertyType;

using std::format;

template<typename ASTNodeType,
    typename std::enable_if<std::is_base_of<INode, ASTNodeType>::value>::type* = nullptr>
using ASTNodePtr = std::shared_ptr<ASTNodeType>;

void Translator::TranslateDDLStatement(const ASTNodePtr<INode>& ddl_statement) {
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
      throw translation_error(format("Unknown DDL statement type: \'{}\'",
                                     ddl_statement->stmt_type.ToString()));
  }
}

void Translator::TranslateCreateDatabaseStatement(const ASTNodePtr<INode>& stmt) {
  auto database_name_node = stmt->Child(0);
  ValidateHasChildren(database_name_node);
  schema_.database_name = GetName(database_name_node);
}
void Translator::TranslateCreateTableStatement(const ASTNodePtr<INode>& stmt) {
  const std::string msg_suffix = " in \'CREATE TABLE\' statement";

  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = GetName(table_name_node);

  ValidateHasChildren(stmt, 2, "Missing table definition" + msg_suffix);
  auto table_definition = stmt->Child(1);

  Node node(table_name);
  ValidateHasChildren(table_definition, 1, "Empty table definition" + msg_suffix);
  auto table_definition_element = table_definition->Child(0);
  if (IsCorrectStmtType(table_definition_element, StmtType::kColumnDef)) {
    std::vector<Property> properties = TranslateColumnDefinitions(table_definition);
    for (auto& property: properties) {
      node.AddProperty(property);
    }
  }
  schema_.AddNode(node);

  for (unsigned i = node.PropertyCount(); HasChildren(table_definition, i + 1); ++i) {
    auto constraint_definition = table_definition->Child(i);
    TranslateTableConstraint(constraint_definition, table_name);
  }
}
void Translator::TranslateAlterTableStatement(const ASTNodePtr<INode>& stmt) {
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
      throw translation_error(format("Unknown action type \'{}\' {}",
                                     action_node->stmt_type.ToString(), msg_suffix));
  }
}
void Translator::TranslateDropDatabaseStatement(const ASTNodePtr<INode>& stmt) {
  for (unsigned i = 0; HasChildren(stmt, i + 1); ++i) {
    auto database_name_node = stmt->Child(i);
    ValidateHasChildren(database_name_node);
    auto database_name = GetName(database_name_node);

    schema_.Clear();
  }
}
void Translator::TranslateDropTableStatement(const ASTNodePtr<INode>& stmt) {
  for (unsigned i = 0; HasChildren(stmt, i + 1); ++i) {
    auto table_name_node = stmt->Child(i);
    ValidateHasChildren(table_name_node);
    auto table_name = GetName(table_name_node);
    schema_.RemoveNode(table_name);
  }
}

void Translator::TranslateAlterTableActionAdd(const ASTNodePtr<INode>& action_node,
                                              std::string& table_name) {
  const std::string msg_suffix = " in \'ALTER TABLE ADD\' statement";

  auto table_definition = action_node->Child(0);

  ValidateHasChildren(table_definition, 1, "Missing column definition or constraint" + msg_suffix);
  auto add_element = table_definition->Child(0);
  unsigned added_columns = 0;
  if (IsCorrectStmtType(add_element, StmtType::kColumnDef)) {
    std::vector<Property> properties = TranslateColumnDefinitions(table_definition);
    for (auto& property: properties) {
      schema_.AddNodeProperty(table_name, property);
    }
    added_columns = properties.size();
  }

  for (unsigned i = added_columns; HasChildren(table_definition, i + 1); ++i) {
    auto constraint_definition = table_definition->Child(i);
    TranslateTableConstraint(constraint_definition, table_name);
  }
}
void Translator::TranslateAlterTableActionDrop(const ASTNodePtr<INode>& action_node,
                                               std::string& table_name) {
  auto alter_drop_list = action_node->Child(0);
  ValidateHasChildren(alter_drop_list, 1, "Missing arguments in \'ALTER TABLE DROP\' statement");
  for (unsigned i = 0; HasChildren(alter_drop_list, i + 1); ++i) {
    auto drop_element = alter_drop_list->Child(i);
    ValidateHasChildren(drop_element);
    TranslateDropElement(drop_element, table_name);
  }
}

std::vector<Property> Translator::TranslateColumnDefinitions(const ASTNodePtr<INode>& node) {
  std::vector<Property> properties;

  for (unsigned i = 0; HasChildren(node, i + 1); ++i) {
    auto column_definition = node->Child(i);
    if (!IsCorrectStmtType(column_definition, StmtType::kColumnDef)) {
      break;
    }
    ValidateHasChildren(column_definition);
    Property property = TranslateColumnDefinition(column_definition);
    properties.push_back(property);
  }

  return properties;
}
Property Translator::TranslateColumnDefinition(const ASTNodePtr<INode>& node) {
  auto column_name_node = node->Child(0);
  ValidateHasChildren(column_name_node);
  std::string column_name = GetName(column_name_node);

  ValidateHasChildren(node, 2, "Missing datatype in column definition");
  StmtType datatype = node->Child(1)->stmt_type;

  PropertyType property_type;
  switch (datatype) {
    case StmtType::kIntType:
      property_type = PropertyType::kInteger;
      break;
    case StmtType::kFloatType:
      property_type = PropertyType::kFloat;
      break;
    case StmtType::kCharType:
    case StmtType::kVarcharType:
      property_type = PropertyType::kString;
      break;
    default:
      throw translation_error(format("Unknown SQL datatype \'{}\'", datatype.ToString()));
  }

  return Property(column_name, property_type);
}
void Translator::TranslateTableConstraint(const ASTNodePtr<INode>& constraint_definition,
                                          const std::string& table_name) {
  ValidateHasChildren(constraint_definition, 1, "Missing constraint definition");

  size_t key_node_number = 1;
  std::string constraint_name;
  if (HasChildren(constraint_definition, key_node_number + 1)) {
    key_node_number++;

    auto constraint_kw = constraint_definition->Child(0);
    ValidateIsCorrectStmtType(constraint_kw, StmtType::kConstraintKW);
    ValidateHasChildren(constraint_kw);
    constraint_name += GetName(constraint_kw->Child(0));
  }

  ValidateHasChildren(constraint_definition, key_node_number, "Missing constraint definition");
  auto key = constraint_definition->Child(key_node_number - 1);
  ValidateHasChildren(key);
  switch (key->stmt_type) {
    case StmtType::kPrimaryKey:
      TranslatePrimaryKey(key, constraint_name, table_name);
      break;
    case StmtType::kForeignKey:
      TranslateForeignKey(key, constraint_name, table_name);
      break;
    default:
      throw translation_error(format("Unknown constraint type \'{}\'", key->stmt_type.ToString()));
  }
}

void Translator::TranslateDropElement(const ASTNodePtr<INode>& node, std::string& table_name) {
  std::vector<std::string> arguments;
  for (unsigned i = 0; HasChildren(node, i + 1); ++i) {
    auto argument_node = node->Child(i);
    ValidateHasChildren(argument_node);
    auto argument_name = GetName(argument_node);
    arguments.push_back(argument_name);
  }

  switch (node->stmt_type) {
    case StmtType::kDropColumn:
      for (const auto& arg: arguments) {
        schema_.RemoveNodeProperty(table_name, arg);
      }
      break;
    case StmtType::kDropConstraint:
      for (const auto& arg: arguments) {
        // TODO: return bool to indicate success and do not find relationship to remove.
        schema_.RemoveNodePropertyConstraints(table_name, arg);
        schema_.RemoveRelationship(arg);
      }
      break;
    default:
      throw translation_error(format("Unknown drop object type \'{}\'",
                                     node->stmt_type.ToString()));
  }
}

} // scc::translator
