#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace scc::translator::cypher;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslateDMLStatement(const NodePtr<INode>& dml_statement) {
  switch (dml_statement->stmt_type) {
    case StmtType::kInsertStmt:
      TranslateInsertStatement(dml_statement);
      break;
    case StmtType::kDeleteStmt:
      TranslateDeleteStatement(dml_statement);
      break;
    case StmtType::kUpdateStmt:
      TranslateUpdateStatement(dml_statement);
      break;
    default:
      throw translation_error(format("Unknown DML statement type: \'{}\'",
                                     dml_statement->stmt_type.ToString()));
  }
}

void Translator::TranslateInsertStatement(const NodePtr<INode>& stmt) {
  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = GetName(table_name_node);

  const auto& schema_node = schema_.FindNodeOrThrow(table_name);
  unsigned properties_count = schema_node.PropertyCount();

  std::string msg_suffix = "in \'INSERT\' statement";

  std::vector<Property> properties(properties_count);
  unsigned child_count = 2;
  ValidateHasChildren(stmt, child_count, format(R"(Expected {} values for '{}' table {})",
                                      properties_count, table_name, msg_suffix));
  auto next_child = stmt->Child(1);
  if (IsCorrectStmtType(next_child, StmtType::kName)) {
    child_count += properties_count;
    ValidateHasChildren(stmt, child_count,
                        format(R"(Expected {} column names for '{}' table {})",
                               properties_count, table_name, msg_suffix));
    for (unsigned i = 1; i < child_count - 1; ++i) {
      auto column_name_node = stmt->Child(i);
      ValidateIsCorrectStmtType(column_name_node, StmtType::kName);
      ValidateHasChildren(column_name_node);
      properties[i - 1].name = GetName(column_name_node);
    }
  }

  ValidateHasChildren(stmt, child_count);
  auto values = stmt->Child(child_count - 1);
  ValidateIsCorrectStmtType(values, StmtType::kValuesKW);
  for (unsigned i = 0; i < properties_count; ++i) {
    auto expression_or_null = values->Child(i);
    auto& property = properties[i];
    if (property.name.empty()) {
      property.name = schema_node.properties[i].name;
    }

    if (IsCorrectStmtType(expression_or_null, StmtType::kNullValue)) {
      property.type = PropertyType::kNull;
      property.value = property.type.ToString();
    } else if (IsCorrectStmtType(expression_or_null, StmtType::kExpression)) {
      auto [type, value] = TranslateExpression(expression_or_null);
      property.type  = type;
      property.value = value;
    } else {
      std::string msg = format("Expected \'NULL\' or expression value {}", msg_suffix);
      throw translation_error(msg);
    }
  }

  Node node(table_name, properties);
  if (schema_node.Validate(node)) {
    WriteCypherQuery(CreateNodeClauseBuilder::Build(node));
  } else {
    std::string msg = format("Node {} does not correspond to node from the schema:\n{}",
                             node.ToString(), schema_node.ToJsonString());
    throw translation_error(msg);
  }
}
void Translator::TranslateUpdateStatement(const NodePtr<INode>& stmt) {
  auto table_name_node = stmt->Child(0);
  ValidateHasChildren(table_name_node);
  std::string table_name = GetName(table_name_node);

  const auto& schema_node = schema_.FindNodeOrThrow(table_name);
  std::vector<Property> properties;

  std::string msg_suffix = "in \'UPDATE\' statement";

  for (unsigned i = 1; i < stmt->ChildrenCount(); ++i) {
    auto update_column_node = stmt->Child(i);
    IsCorrectStmtType(update_column_node, StmtType::kUpdateColumn);
    ValidateHasChildren(update_column_node, 2);

    Property property;

    auto column_name_node = update_column_node->Child(0);
    ValidateIsCorrectStmtType(column_name_node, StmtType::kName);
    property.name = GetName(column_name_node);

    auto expression_or_null = update_column_node->Child(1);
    if (IsCorrectStmtType(expression_or_null, StmtType::kNullValue)) {
      property.type = PropertyType::kNull;
      property.value = property.type.ToString();
    } else if (IsCorrectStmtType(expression_or_null, StmtType::kExpression)) {
      auto [type, value] = TranslateExpression(expression_or_null);
      property.type  = type;
      property.value = value;
    } else {
      std::string msg = format("Expected \'NULL\' or expression value {}", msg_suffix);
      throw translation_error(msg);
    }

    properties.push_back(property);
  }

  for (const auto& property : properties) {
    try {
      const auto& schema_property = schema_node.FindPropertyOrThrow(property.name);
      if (!schema_property.Validate(property)) {
        std::string msg = format("Property {} does not correspond to property from the schema:\n{}",
                                 property.name, schema_property.ToJsonString());
        throw translation_error(msg);
      } else {
        WriteCypherQuery(SetPropertyClauseBuilder::Build(table_name, property));
      }
    } catch (const std::runtime_error& error) {
      std::string msg = format("Property {} does not exist in the node \'{}\' from the schema",
                               property.name, table_name);
      throw translation_error(msg);
    }
  }
}
void Translator::TranslateDeleteStatement(const NodePtr<INode>& stmt) {
  ValidateHasChildren(stmt, 1, "Empty \'DELETE\' statement");
}

} // scc::translator
