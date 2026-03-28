#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace ast::common;
using namespace schema;

using std::format;
using cypher::ConstraintType;
using cypher::PropertyType;

template<typename ASTNodeType,
    typename std::enable_if<std::is_base_of<INode, ASTNodeType>::value>::type* = nullptr>
using ASTNodePtr = std::shared_ptr<ASTNodeType>;

void Translator::TranslatePrimaryKey(const ASTNodePtr<INode>& primary_key,
                                     const std::string& constraint_name,
                                     const std::string& table_name) {
  for (unsigned i = 0; HasChildren(primary_key, i + 1); ++i) {
    auto column_name_node = primary_key->Child(i);
    ValidateHasChildren(column_name_node);
    auto column_name = GetName(column_name_node);

    std::string constraint_name_prefix = constraint_name.empty()
        ? CreatePKConstraintPrefix(table_name) : constraint_name;
    AddPropertyConstraints(constraint_name_prefix, table_name, column_name,
                           {ConstraintType::kUniqueness, ConstraintType::kExistence});
  }
}
void Translator::TranslateForeignKey(const ASTNodePtr<INode>& foreign_key,
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

  if (properties.size() != ref_columns.size()) {
    std::string msg = format("Wrong number of columns for foreign key. Expected {}, got {}",
                                  properties.size(), ref_columns.size());
    throw translation_error(msg);
  }

  // TODO: Match all nodes with correspond labels and remove properties.
//  for (const auto& property: properties) {
//    RemoveProperty(table_name, property);
//  }
//  for (const auto& ref_property: ref_columns) {
//    RemoveProperty(ref_table_name, ref_property);
//  }

  auto apply_conditions = CreateApplyConditions(table_name, properties,
                                                ref_table_name, ref_columns);
  std::string relationship_type = constraint_name.empty()
      ? CreateRelationshipType(CreateFKConstraintPrefix(table_name, ref_table_name))
      : constraint_name;
  AddRelationship(relationship_type, table_name, ref_table_name, apply_conditions);
}

std::string Translator::GetName(const ASTNodePtr<INode>& name_node) {
  std::ostringstream name;
  ValidateHasChildren(name_node);
  for (unsigned i = 0; HasChildren(name_node, i + 1); ++i) {
    auto identifier_node = name_node->Child(i);
    auto identifier = GetIdentifier(identifier_node);
    name << identifier;
  }
  return name.str();
}
std::string Translator::GetIdentifier(const ASTNodePtr<INode>& node) {
  return ASTUtils::CastToNodeType<StringNode>(node->Child(0))->data;
}

void Translator::AddPropertyConstraints(const std::string& constraint_name_prefix,
                                        const std::string& label, const std::string& property_name,
                                        const std::vector<ConstraintType>& constraint_types) {
  for (const auto& constraint_type : constraint_types) {
    std::string constraint_name = format("{}{}", constraint_name_prefix, constraint_counter);
    schema_.AddNodePropertyConstraint(label, property_name,
                                      PropertyConstraint(constraint_name, constraint_type));
    constraint_counter++;
  }
}
void Translator::AddRelationship(const std::string& relationship_type,
                                 const std::string& start_label, const std::string& end_label,
                                 const std::vector<ApplyCondition>& apply_conditions) {
  Relationship relationship(relationship_type, start_label, end_label, apply_conditions);
  schema_.AddRelationship(relationship);
}

std::vector<ApplyCondition> Translator::CreateApplyConditions(
    const std::string& start_label, const std::vector<std::string>& start_node_props,
    const std::string& end_label, const std::vector<std::string>& end_node_props
) {
  const auto& start_node = GetNodeFromSchema(start_label);
  const auto& end_node = GetNodeFromSchema(end_label);
  std::vector<ApplyCondition> apply_conditions;
  for (unsigned i = 0; i < start_node_props.size(); ++i) {
    int spi = start_node.PropertyIndex(start_node_props[i]);
    int epi = end_node.PropertyIndex(end_node_props[i]);
    apply_conditions.push_back({spi, epi});
  }
  return apply_conditions;
}
std::string Translator::CreateRelationshipType(const std::string& type_prefix) {
  std::string type = format("{}{}", type_prefix, relationship_counter);
  relationship_counter++;
  return type;
}
std::string Translator::CreatePKConstraintPrefix(const std::string& table_name) {
  return format("{}_pk_", table_name);
}
std::string Translator::CreateFKConstraintPrefix(const std::string& table_name,
                                                 const std::string& ref_table_name) {
  return format("fk_{}_to_{}_", table_name, ref_table_name);
}

const Node& Translator::GetNodeFromSchema(const std::string& label) const {
  try {
    const Node& node = schema_.FindNodeOrThrow(label);
    return node;
  } catch (std::runtime_error& e) {
    std::string msg = format("Table {} not found", label);
    throw translation_error(msg);
  }
}

} // scc::translator
