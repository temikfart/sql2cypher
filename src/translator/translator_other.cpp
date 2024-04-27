#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

// Basic statements

void Translator::TranslatePrimaryKey(
    std::shared_ptr<INode> key,
    std::string& constraint_name,
    std::string& table_name) {
  if (key->stmt_type != StmtType::kPrimaryKey) {
    LOGE << "incorrect type for primaryKey node";
    end(EXIT_FAILURE);
  }
  if (key->ChildrenCount() == 0) {
    LOGE << "PRIMARY KEY definition is missed";
    end(EXIT_FAILURE);
  }

  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(key->get_child(0)));
  if (key->ChildrenCount() > 1) {
    std::vector<std::string> other_properties =
        this->GetListOf(key->get_child(1), StmtType::kIdentifier);
    properties.insert(properties.end(),
                      other_properties.begin(),
                      other_properties.end());
  }

  this->CreateUniqueNodePropertyConstraint(
      (constraint_name + "_" + std::to_string(constraint_counter++)),
      table_name,
      properties
  );
  for (auto& i: properties) {
    this->CreateNodePropertyExistenceConstraint(
        (constraint_name + "_" + std::to_string(constraint_counter++)),
        table_name,
        i
    );
  }
}
void Translator::TranslateForeignKey(
    std::shared_ptr<INode> key,
    std::string& table_name) {
  if (key->stmt_type != StmtType::kForeignKey) {
    LOGE << "incorrect type for foreignKey node";
    end(EXIT_FAILURE);
  }
  if (key->ChildrenCount() == 0) {
    LOGE << "PRIMARY KEY definition is missed";
    end(EXIT_FAILURE);
  }
  int reference_child_num = 1;

  std::vector<std::string> properties;
  properties.push_back(this->TranslateName(key->get_child(0)));
  if (key->ChildrenCount() > 2) {
    if (key->get_child(1)->stmt_type != StmtType::kCommaDelimiter) {
      LOGE << "invalid delimiter between properties in foreign key";
      end(EXIT_FAILURE);
    }
    reference_child_num++;
    std::vector<std::string> other_properties =
        this->GetListOf(key->get_child(1), StmtType::kName);
    properties.insert(properties.end(),
                      other_properties.begin(),
                      other_properties.end());
  }

  // Get reference
  auto reference = key->get_child(reference_child_num);
  if (reference->stmt_type != StmtType::kReferencesKW) {
    LOGE << "invalid foreign key: incorrect reference statement type";
    end(EXIT_FAILURE);
  }
  if (reference->ChildrenCount() == 0) {
    LOGE << "empty reference";
    end(EXIT_FAILURE);
  }
  auto table_name_node = reference->get_child(0);
  std::string ref_table_name = this->TranslateName(table_name_node);

  // Get ref columns if present
  std::vector<std::string> ref_columns;
  if (reference->ChildrenCount() > 1) {
    ref_columns.push_back(this->TranslateName(reference->get_child(1)));
    if (reference->ChildrenCount() > 2) {
      std::vector<std::string> other_props =
          this->GetListOf(reference->get_child(2),
                          StmtType::kName);
      ref_columns.insert(ref_columns.end(),
                         other_props.begin(),
                         other_props.end());
    }
  }

//  this->RemoveProperties(table_name, properties);
//  this->RemoveProperties(table_name, ref_columns);
  this->CreateRelationship(table_name, ref_table_name);
}
void Translator::CreateUniqueNodePropertyConstraint(
    const std::string& constraint_name,
    const std::string& LabelName,
    const std::vector<std::string>& properties) {
  out_ << "CREATE CONSTRAINT " << constraint_name
       << " IF NOT EXISTS" << std::endl;
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
void Translator::CreateNodePropertyExistenceConstraint(
    const std::string& constraint_name,
    const std::string& LabelName,
    const std::string& property) {
  out_ << "CREATE CONSTRAINT " << constraint_name
       << " IF NOT EXISTS" << std::endl;
  out_ << "FOR (n:" << LabelName << ")" << std::endl;
  out_ << "REQUIRE (n." << property << ") IS NOT NULL;\n" << std::endl;
}
void Translator::CreateRelationship(
    const std::string& label_name,
    const std::string& ref_label_name) {
  out_ << "MATCH (a:" << label_name << "), (b:" << ref_label_name << ")\n";
  out_ << "CREATE (a)-[r:fk_"
       << label_name << "_to_" << ref_label_name << "_" << relationship_counter
       << "]->(b);\n" << std::endl;
  relationship_counter++;
}
void Translator::RemoveProperties(
    const std::string& label_name,
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

std::vector<std::string> Translator::GetListOf(
    std::shared_ptr<INode> node,
    StmtType type) {
  if (node->stmt_type != StmtType::kCommaDelimiter) {
    LOGE << "invalid ListOf: delimiter is not a comma";
    end(EXIT_FAILURE);
  }
  if (node->ChildrenCount() == 0) {
    LOGE << "invalid ListOf: comma without children";
    end(EXIT_FAILURE);
  }

  std::vector<std::string> arguments;
  switch (type) {
    case StmtType::kName:
      arguments.push_back(this->TranslateName(node->get_child(0)));
      break;
    case StmtType::kIdentifier:
      arguments.push_back(this->TranslateIdentifier(node->get_child(0)));
      break;
    default:
      LOGE << "invalid ListOf: unknown argument type";
      end(EXIT_FAILURE);
  }

  if (node->ChildrenCount() > 1) {
    std::vector<std::string> other_arguments =
        this->GetListOf(node->get_child(1), type);
    arguments.insert(arguments.end(),
                     other_arguments.begin(),
                     other_arguments.end());
  }

  return arguments;
}

std::string Translator::TranslateName(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "empty name node";
    end(EXIT_FAILURE);
  }

  std::ostringstream name;

  name << this->TranslateIdentifier(node->get_child(0));
  if (node->ChildrenCount() > 1) {
    if (node->get_child(1)->stmt_type == StmtType::kDotDelimiter) {
      name << this->TranslateIdentifiers(node->get_child(1));
    } else {
      LOGE << "invalid name: delimiter is not a dot";
      end(EXIT_FAILURE);
    }
  }

  return name.str();
}
std::string Translator::TranslateIdentifiers(
    std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "invalid list of identifiers";
    end(EXIT_FAILURE);
  }

  std::ostringstream identifiers;
  identifiers << "." << this->TranslateIdentifier(node->get_child(0));

  if (node->ChildrenCount() > 1) {
    identifiers << this->TranslateIdentifiers(node->get_child(1));
  }

  return identifiers.str();
}
std::string Translator::TranslateIdentifier(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "empty identifier";
    end(EXIT_FAILURE);
  }

  return std::dynamic_pointer_cast<StringNode>(node->get_child(0))->data;
}

} // scc::translator
