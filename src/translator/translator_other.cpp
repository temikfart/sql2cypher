#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace ast::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslatePrimaryKey(const NodePtr<INode>& key, std::string& constraint_name,
                                     std::string& table_name) {
  std::vector<std::string> properties;
  auto column_name_node = key->get_child(0);
  ValidateHasChildren(column_name_node);
  properties.push_back(TranslateName(column_name_node));

  if (HasChildren(key, 2)) {
    auto other_column_name_node = key->get_child(1);
    ValidateHasChildren(other_column_name_node);
    std::vector<std::string> other_properties = GetListOf(other_column_name_node, StmtType::kName);
    properties.insert(properties.end(), other_properties.begin(), other_properties.end());
  }

  CreateUniqueNodePropertyConstraint(
      constraint_name + "_" + std::to_string(constraint_counter++),
      table_name,
      properties
  );
  for (auto& i: properties) {
    CreateNodePropertyExistenceConstraint(
        constraint_name + "_" + std::to_string(constraint_counter++),
        table_name,
        i
    );
  }
}
void Translator::TranslateForeignKey(const NodePtr<INode>& key, std::string& table_name) {
  int reference_child_num = 1;

  std::vector<std::string> properties;
  auto column_name_node = key->get_child(0);
  ValidateHasChildren(column_name_node);
  properties.push_back(TranslateName(column_name_node));

  if (HasChildren(key, 3)) {
    reference_child_num++;
    auto separator_node = key->get_child(1);
    ValidateIsCorrectStmtType(separator_node, StmtType::kCommaDelimiter);
    std::vector<std::string> other_properties = GetListOf(separator_node, StmtType::kName);
    properties.insert(properties.end(), other_properties.begin(), other_properties.end());
  }

  auto reference = key->get_child(reference_child_num);
  ValidateIsCorrectStmtType(reference, StmtType::kReferencesKW);

  ValidateHasChildren(reference);
  auto table_name_node = reference->get_child(0);

  ValidateHasChildren(table_name_node);
  std::string ref_table_name = TranslateName(table_name_node);

  std::vector<std::string> ref_columns;
  if (HasChildren(reference, 2)) {
    auto ref_column_name_node = reference->get_child(1);
    ValidateHasChildren(ref_column_name_node);
    ref_columns.push_back(TranslateName(ref_column_name_node));
    if (HasChildren(reference, 3)) {
      auto other_ref_column_name_node = reference->get_child(2);
      ValidateHasChildren(other_ref_column_name_node);
      std::vector<std::string> other_props = GetListOf(other_ref_column_name_node, StmtType::kName);
      ref_columns.insert(ref_columns.end(), other_props.begin(), other_props.end());
    }
  }

//  RemoveProperties(table_name, properties);
//  RemoveProperties(table_name, ref_columns);
  CreateRelationship(table_name, ref_table_name);
}
void Translator::CreateUniqueNodePropertyConstraint(const std::string& constraint_name,
                                                    const std::string& LabelName,
                                                    const std::vector<std::string>& properties) {
  out_ << "CREATE CONSTRAINT " << constraint_name << " IF NOT EXISTS" << std::endl;
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
void Translator::CreateNodePropertyExistenceConstraint(const std::string& constraint_name,
                                                       const std::string& LabelName,
                                                       const std::string& property) {
  out_ << "CREATE CONSTRAINT " << constraint_name << " IF NOT EXISTS" << std::endl;
  out_ << "FOR (n:" << LabelName << ")" << std::endl;
  out_ << "REQUIRE (n." << property << ") IS NOT NULL;\n" << std::endl;
}
void Translator::CreateRelationship(const std::string& label_name,
                                    const std::string& ref_label_name) {
  out_ << "MATCH (a:" << label_name << "), (b:" << ref_label_name << ")\n";
  out_ << "CREATE (a)-[r:fk_" << label_name
       << "_to_" << ref_label_name << "_" << relationship_counter
       << "]->(b);\n" << std::endl;
  relationship_counter++;
}
void Translator::RemoveProperties(const std::string& label_name,
                                  const std::vector<std::string>& properties) {
  if (properties.empty()) {
    return;
  }
  out_ << "MATCH (n:" << label_name << ")\n";
  out_ << "SET ";
  for (size_t i = 0; i < properties.size(); i++) {
    out_ << "n." << properties[i] << " = null";
    if (i + 1 != properties.size()) {
      out_ << ", ";
    }
  }
  out_ << ";\n" << std::endl;
}

std::vector<std::string> Translator::GetListOf(const NodePtr<INode>& node, StmtType type) {
  std::vector<std::string> arguments;
  switch (type) {
    case StmtType::kName:
      arguments.push_back(TranslateName(node->get_child(0)));
      break;
    case StmtType::kIdentifier:
      arguments.push_back(TranslateIdentifier(node->get_child(0)));
      break;
    default:
      LOGE << "invalid ListOf: unknown argument type";
      end(EXIT_FAILURE);
  }

  if (HasChildren(node, 2)) {
    auto next_separator_node = node->get_child(1);
    ValidateHasChildren(next_separator_node);
    std::vector<std::string> other_arguments = GetListOf(next_separator_node, type);
    arguments.insert(arguments.end(), other_arguments.begin(), other_arguments.end());
  }

  return arguments;
}

std::string Translator::TranslateName(const NodePtr<INode>& node) {
  std::ostringstream name;
  auto identifier_node = node->get_child(0);
  ValidateHasChildren(identifier_node);
  name << TranslateIdentifier(identifier_node);
  if (HasChildren(node, 2)) {
    auto dot_delimiter_node = node->get_child(1);
    ValidateIsCorrectStmtType(dot_delimiter_node, StmtType::kDotDelimiter);
    name << TranslateIdentifiers(dot_delimiter_node);
  }

  return name.str();
}
std::string Translator::TranslateIdentifiers(const NodePtr<INode>& node) {
  std::ostringstream identifiers;
  auto identifier_node = node->get_child(0);
  ValidateHasChildren(identifier_node);
  identifiers << "." << TranslateIdentifier(identifier_node);

  if (HasChildren(node, 2)) {
    auto dot_delimiter_node = node->get_child(1);
    identifiers << TranslateIdentifiers(dot_delimiter_node);
  }

  return identifiers.str();
}
std::string Translator::TranslateIdentifier(const NodePtr<INode>& node) {
  return ASTUtils::CastToNodeType<StringNode>(node->get_child(0))->data;
}

} // scc::translator
