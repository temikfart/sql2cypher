#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace ast::common;
using namespace cypher;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslatePrimaryKey(const NodePtr<INode>& primary_key,
                                     const std::string& constraint_name,
                                     const std::string& table_name) {
  for (unsigned i = 0; HasChildren(primary_key, i + 1); ++i) {
    auto column_name_node = primary_key->Child(i);
    ValidateHasChildren(column_name_node);
    auto column_name = GetName(column_name_node);

    std::string constraint_name_prefix = constraint_name.empty()
        ? CreatePrimaryKeyConstraintName(table_name) : constraint_name;
    CreateConstraint(constraint_name_prefix + "_" + std::to_string(constraint_counter++),
                     table_name, column_name, ConstraintType::kUniqueness);
    CreateConstraint(constraint_name_prefix + "_" + std::to_string(constraint_counter++),
                     table_name, column_name, ConstraintType::kExistence);
  }
}
void Translator::TranslateForeignKey(const NodePtr<INode>& foreign_key,
                                     const std::string& constraint_name,
                                     const std::string& table_name) {
  std::vector<std::string> properties;
  for (unsigned i = 0; HasChildren(foreign_key, i + 1); ++i) {
    auto column_name_node = foreign_key->Child(i);
    if (!IsCorrectStmtType(column_name_node, StmtType::kName)) {
      break;
    }
    ValidateHasChildren(column_name_node);
    auto column_name = GetName(column_name_node);
    properties.push_back(column_name);
  }

  ValidateHasChildren(foreign_key, properties.size() + 1);
  auto reference = foreign_key->Child(properties.size());
  ValidateIsCorrectStmtType(reference, StmtType::kReferencesKW);

  ValidateHasChildren(reference);
  auto table_name_node = reference->Child(0);

  ValidateHasChildren(table_name_node);
  std::string ref_table_name = GetName(table_name_node);

  std::vector<std::string> ref_columns;
  for (unsigned i = 1; HasChildren(reference, i + 1); ++i) {
    auto ref_column_name_node = reference->Child(i);
    if (!IsCorrectStmtType(ref_column_name_node, StmtType::kName)) {
      break;
    }
    ValidateHasChildren(ref_column_name_node);
    auto column_name = GetName(ref_column_name_node);
    ref_columns.push_back(column_name);
  }

  // TODO: Match all nodes with correspond labels and remove properties.
//  for (const auto& property: properties) {
//    RemoveProperty(table_name, property);
//  }
//  for (const auto& ref_property: ref_columns) {
//    RemoveProperty(ref_table_name, ref_property);
//  }
  std::string relationship_type = constraint_name.empty()
      ? CreateRelationshipType(CreatePrimaryKeyConstraintName(table_name)) : constraint_name;
  CreateRelationship(relationship_type, table_name, ref_table_name);
}

std::string Translator::GetName(const NodePtr<INode>& name_node) const {
  std::ostringstream name;
  ValidateHasChildren(name_node);
  for (unsigned i = 0; HasChildren(name_node, i + 1); ++i) {
    auto identifier_node = name_node->Child(i);
    auto identifier = GetIdentifier(identifier_node);
    name << identifier;
  }
  return name.str();
}
std::string Translator::GetIdentifier(const NodePtr<INode>& node) const {
  return ASTUtils::CastToNodeType<StringNode>(node->Child(0))->data;
}

void Translator::CreateConstraint(const std::string& constraint_name,
                                  const std::string& label_name,
                                  const std::string& property_name,
                                  ConstraintType constraint_type) {
  Node node(label_name);
  NodeProperty property(property_name, std::string(stub_str), PropertyType::kUnknown);

  WriteCypherQuery(
      CreateConstraintClauseBuilder::Build(constraint_name, node, property, constraint_type)
  );
}
void Translator::CreateRelationship(const std::string& relationship_type,
                                    const std::string& start_label_name,
                                    const std::string& end_label_name) {
  Node start_node(start_label_name, "a"), end_node(end_label_name, "b");
  Relationship relationship(relationship_type, start_node, end_node,
                            Relationship::Direction::kRight);
  WriteCypherQuery(CreateRelationshipClauseBuilder::Build(relationship));
}
void Translator::RemoveProperty(const std::string& label_name, const std::string& property_name) {
  Node node(label_name, "n");
  WriteCypherQuery(RemovePropertyClauseBuilder::Build(node, property_name));
}
std::string Translator::CreateRelationshipType(const std::string& type_prefix) {
  std::stringstream ss;
  ss << type_prefix << "_" << relationship_counter;
  relationship_counter++;

  return ss.str();
}
std::string Translator::CreatePrimaryKeyConstraintName(const std::string& table_name) const {
  return table_name + "_constraint";
}
std::string Translator::CreateForeignKeyConstraintName(const std::string& table_name,
                                                       const std::string& ref_table_name) const {
  return "fk_" + table_name + "_to_" + ref_table_name;
}

} // scc::translator
