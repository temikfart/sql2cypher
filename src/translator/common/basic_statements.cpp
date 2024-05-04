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
  auto column_name_node = primary_key->Child(0);
  ValidateHasChildren(column_name_node);
  auto column_name = GetName(column_name_node);

  std::vector<std::string> properties;
  properties.push_back(column_name);

  if (HasChildren(primary_key, 2)) {
    auto other_column_name_node = primary_key->Child(1);
    ValidateHasChildren(other_column_name_node);
    std::vector<std::string> other_properties = GetList(other_column_name_node, StmtType::kName);
    properties.insert(properties.end(), other_properties.begin(), other_properties.end());
  }

  for (const auto& property: properties) {
    CreateConstraint(constraint_name + "_" + std::to_string(constraint_counter++), table_name,
                        property, ConstraintType::kUniqueness);
    CreateConstraint(constraint_name + "_" + std::to_string(constraint_counter++), table_name,
                        property, ConstraintType::kExistence);
  }
}
void Translator::TranslateForeignKey(const NodePtr<INode>& foreign_key,
                                     const std::string& table_name) {
  auto column_name_node = foreign_key->Child(0);
  ValidateHasChildren(column_name_node);
  auto column_name = GetName(column_name_node);

  std::vector<std::string> properties;
  properties.push_back(column_name);

  int reference_child_num = 1;
  if (HasChildren(foreign_key, 3)) {
    reference_child_num++;
    auto separator_node = foreign_key->Child(1);
    ValidateIsCorrectStmtType(separator_node, StmtType::kCommaDelimiter);
    std::vector<std::string> other_properties = GetList(separator_node, StmtType::kName);
    properties.insert(properties.end(), other_properties.begin(), other_properties.end());
  }

  auto reference = foreign_key->Child(reference_child_num);
  ValidateIsCorrectStmtType(reference, StmtType::kReferencesKW);

  ValidateHasChildren(reference);
  auto table_name_node = reference->Child(0);

  ValidateHasChildren(table_name_node);
  std::string ref_table_name = GetName(table_name_node);

  std::vector<std::string> ref_columns;
  if (HasChildren(reference, 2)) {
    auto ref_column_name_node = reference->Child(1);
    ValidateHasChildren(ref_column_name_node);
    ref_columns.push_back(GetName(ref_column_name_node));
    if (HasChildren(reference, 3)) {
      auto other_ref_column_name_node = reference->Child(2);
      ValidateHasChildren(other_ref_column_name_node);
      std::vector<std::string> other_props = GetList(other_ref_column_name_node, StmtType::kName);
      ref_columns.insert(ref_columns.end(), other_props.begin(), other_props.end());
    }
  }

  // TODO: Match all nodes with correspond labels and remove properties.
//  for (const auto& property: properties) {
//    RemoveProperty(table_name, property);
//  }
//  for (const auto& ref_property: ref_columns) {
//    RemoveProperty(ref_table_name, ref_property);
//  }
  CreateRelationship(table_name, ref_table_name);
}

std::vector<std::string> Translator::GetList(const NodePtr<INode>& node, StmtType type) const {
  auto argument_node = node->Child(0);
  ValidateHasChildren(argument_node);
  std::vector<std::string> arguments;
  std::string argument;
  switch (type) {
    case StmtType::kName:
      argument = GetName(argument_node);
      break;
    case StmtType::kIdentifier:
      argument = GetIdentifier(argument_node);
      break;
    default:
      throw translation_error("Unknown argument type for list of elements");
  }
  arguments.push_back(argument);

  if (HasChildren(node, 2)) {
    auto next_separator_node = node->Child(1);
    ValidateHasChildren(next_separator_node);
    std::vector<std::string> other_arguments = GetList(next_separator_node, type);
    arguments.insert(arguments.end(), other_arguments.begin(), other_arguments.end());
  }

  return arguments;
}
std::string Translator::GetName(const NodePtr<INode>& node) const {
  auto identifier_node = node->Child(0);
  ValidateHasChildren(identifier_node);

  std::ostringstream name;
  name << GetIdentifier(identifier_node);

  if (HasChildren(node, 2)) {
    auto dot_delimiter_node = node->Child(1);
    ValidateHasChildren(dot_delimiter_node);
    ValidateIsCorrectStmtType(dot_delimiter_node, StmtType::kDotDelimiter);
    name << GetIdentifiersJoinedByDot(dot_delimiter_node);
  }

  return name.str();
}
std::string Translator::GetIdentifiersJoinedByDot(const NodePtr<INode>& node) const {
  auto identifier_node = node->Child(0);
  ValidateHasChildren(identifier_node);

  std::ostringstream identifiers;
  identifiers << "." << GetIdentifier(identifier_node);

  if (HasChildren(node, 2)) {
    auto dot_delimiter_node = node->Child(1);
    ValidateHasChildren(dot_delimiter_node);
    ValidateIsCorrectStmtType(dot_delimiter_node, StmtType::kIdentifier);
    identifiers << GetIdentifiersJoinedByDot(dot_delimiter_node);
  }

  return identifiers.str();
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
void Translator::CreateRelationship(const std::string& start_label_name,
                                    const std::string& end_label_name) {
  Node start_node(start_label_name, "a"), end_node(end_label_name, "b");

  std::string relationship_type = CreateRelationshipType(start_label_name, end_label_name);
  Relationship relationship(relationship_type, start_node, end_node,
                            Relationship::Direction::kRight);

  WriteCypherQuery(CreateRelationshipClauseBuilder::Build(relationship));
}
void Translator::RemoveProperty(const std::string& label_name, const std::string& property_name) {
  Node node(label_name);
  WriteCypherQuery(RemovePropertyClauseBuilder::Build(node, property_name));
}
std::string Translator::CreateRelationshipType(const std::string& start_label_name,
                                               const std::string& end_label_name) {
  std::stringstream ss;
  ss << "fk_" << start_label_name << "_to_" << end_label_name << "_" << relationship_counter;
  relationship_counter++;

  return ss.str();
}

} // scc::translator
