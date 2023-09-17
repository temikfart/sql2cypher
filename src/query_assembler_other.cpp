#include "SCC/query_assembler.h"

// Basic statements

void QueryAssembler::TranslatePrimaryKey(
    std::shared_ptr<Node> key,
    std::string& constraint_name,
    std::string& table_name) {
  if (key->get_st_type() != StatementType::primaryKey) {
    LOGE << "incorrect type for primaryKey node";
    end(EXIT_FAILURE);
  }
  if (key->get_children_amount() == 0) {
    LOGE << "PRIMARY KEY definition is missed";
    end(EXIT_FAILURE);
  }

  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(key->get_child(0)));
  if (key->get_children_amount() > 1) {
    std::vector<std::string> other_properties =
        this->GetListOf(key->get_child(1), StatementType::identifier);
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
void QueryAssembler::TranslateForeignKey(
    std::shared_ptr<Node> key,
    std::string& table_name) {
  if (key->get_st_type() != StatementType::foreignKey) {
    LOGE << "incorrect type for foreignKey node";
    end(EXIT_FAILURE);
  }
  if (key->get_children_amount() == 0) {
    LOGE << "PRIMARY KEY definition is missed";
    end(EXIT_FAILURE);
  }
  int reference_child_num = 1;

  std::vector<std::string> properties;
  properties.push_back(this->TranslateIdentifier(key->get_child(0)));
  if (key->get_children_amount() > 2) {
    if (key->get_child(1)->get_st_type() != StatementType::delimiter_comma) {
      LOGE << "invalid delimiter between properties in foreign key";
      end(EXIT_FAILURE);
    }
    reference_child_num++;
    std::vector<std::string> other_properties =
        this->GetListOf(key->get_child(1), StatementType::identifier);
    properties.insert(properties.end(),
                      other_properties.begin(),
                      other_properties.end());
  }

  // Get reference
  auto reference = key->get_child(reference_child_num);
  if (reference->get_st_type() != StatementType::reference) {
    LOGE << "invalid foreign key: incorrect referene statement type";
    end(EXIT_FAILURE);
  }
  if (reference->get_children_amount() == 0) {
    LOGE << "empty reference";
    end(EXIT_FAILURE);
  }
  auto table_name_node = reference->get_child(0);
  std::string ref_table_name = this->TranslateName(table_name_node);

  // Get ref columns if present
  std::vector<std::string> ref_columns;
  if (reference->get_children_amount() > 1) {
    ref_columns.push_back(this->TranslateIdentifier(reference->get_child(1)));
    if (reference->get_children_amount() > 2) {
      std::vector<std::string> other_props =
          this->GetListOf(reference->get_child(2),
                          StatementType::identifier);
      ref_columns.insert(ref_columns.end(),
                         other_props.begin(),
                         other_props.end());
    }
  }

//  this->RemoveProperties(table_name, properties);
//  this->RemoveProperties(table_name, ref_columns);
  this->CreateRelationship(table_name, ref_table_name);
}
void QueryAssembler::CreateUniqueNodePropertyConstraint(
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
void QueryAssembler::CreateNodePropertyExistenceConstraint(
    const std::string& constraint_name,
    const std::string& LabelName,
    const std::string& property) {
  out_ << "CREATE CONSTRAINT " << constraint_name
       << " IF NOT EXISTS" << std::endl;
  out_ << "FOR (n:" << LabelName << ")" << std::endl;
  out_ << "REQUIRE (n." << property << ") IS NOT NULL;\n" << std::endl;
}
void QueryAssembler::CreateRelationship(
    const std::string& label_name,
    const std::string& ref_label_name) {
  out_ << "MATCH (a:" << label_name << "), (b:" << ref_label_name << ")\n";
  out_ << "CREATE (a)-[r:fk_"
       << label_name << "_to_" << ref_label_name << "_" << relationship_counter
       << "]->(b);\n" << std::endl;
  relationship_counter++;
}
void QueryAssembler::RemoveProperties(
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

std::vector<std::string> QueryAssembler::GetListOf(
    std::shared_ptr<Node> node,
    StatementType type) {
  if (node->get_st_type() != StatementType::delimiter_comma) {
    LOGE << "invalid ListOf: delimiter is not a comma";
    end(EXIT_FAILURE);
  }
  if (node->get_children_amount() == 0) {
    LOGE << "invalid ListOf: comma without children";
    end(EXIT_FAILURE);
  }

  std::vector<std::string> arguments;
  switch (type) {
    case StatementType::name:
      arguments.push_back(this->TranslateName(node->get_child(0)));
      break;
    case StatementType::identifier:
      arguments.push_back(this->TranslateIdentifier(node->get_child(0)));
      break;
    default:
      LOGE << "invalid ListOf: unknown argument type";
      end(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    std::vector<std::string> other_arguments =
        this->GetListOf(node->get_child(1), type);
    arguments.insert(arguments.end(),
                     other_arguments.begin(),
                     other_arguments.end());
  }

  return arguments;
}

std::string QueryAssembler::TranslateName(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "empty name node";
    end(EXIT_FAILURE);
  }

  std::ostringstream name;

  name << this->TranslateIdentifier(node->get_child(0));
  if (node->get_children_amount() > 1) {
    if (node->get_child(1)->get_st_type() == StatementType::delimiter_dot) {
      name << this->TranslateIdentifiers(node->get_child(1));
    } else {
      LOGE << "invalid name: delimiter is not a dot";
      end(EXIT_FAILURE);
    }
  }

  return name.str();
}
std::string QueryAssembler::TranslateIdentifiers(
    std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "invalid list of identifiers";
    end(EXIT_FAILURE);
  }

  std::ostringstream identifiers;
  identifiers << "." << this->TranslateIdentifier(node->get_child(0));

  if (node->get_children_amount() > 1) {
    identifiers << this->TranslateIdentifiers(node->get_child(1));
  }

  return identifiers.str();
}
std::string QueryAssembler::TranslateIdentifier(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "empty identifier";
    end(EXIT_FAILURE);
  }

  return std::dynamic_pointer_cast<StringNode>(node->get_child(0))->get_data();
}
