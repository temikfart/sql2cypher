#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <memory>
#include <tuple>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <utility>

#include "SCC/ast/nodes/inode.h"
#include "SCC/config/scc_config.h"
#include "SCC/log/log.h"
#include "SCC/parser/parser.h"
#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"
#include "SCC/translator/schema/relationship.h"
#include "SCC/translator/schema/schema.h"

#include "logger/log.hpp"

namespace scc::translator {

class translation_error : public std::logic_error {
public:
  explicit translation_error(const std::string& message);
};

class Translator {
public:
  explicit Translator(std::shared_ptr<ast::INode> ast, const std::filesystem::path& out_path);
  explicit Translator(std::shared_ptr<ast::INode> ast, std::filesystem::path graph_schema_path,
                      const std::filesystem::path& out_path);

  void Translate();

private:
  std::shared_ptr<ast::INode> ast_;
  bool translate_schema_ = false;
  bool translate_data_ = false;
  schema::Schema schema_;
  std::filesystem::path schema_path_;
  std::ofstream out_;

  int constraint_counter = 0;
  int relationship_counter = 0;

  void WriteCypherQuery(const std::string& query);
  void FinishCypherQueriesGroup();

  void TranslateQuery(const std::shared_ptr<ast::INode>& query);

  void TranslateDDLStatement(const std::shared_ptr<ast::INode>& ddl_statement);
  void TranslateDMLStatement(const std::shared_ptr<ast::INode>& dml_statement);

  // DDL statements

  void TranslateCreateDatabaseStatement(const std::shared_ptr<ast::INode>& stmt);
  void TranslateCreateTableStatement(const std::shared_ptr<ast::INode>& stmt);
  void TranslateAlterTableStatement(const std::shared_ptr<ast::INode>& stmt);
  void TranslateDropDatabaseStatement(const std::shared_ptr<ast::INode>& stmt);
  void TranslateDropTableStatement(const std::shared_ptr<ast::INode>& stmt);

  void TranslateAlterTableActionAdd(const std::shared_ptr<ast::INode>& action_node,
                                    std::string& table_name);
  void TranslateAlterTableActionDrop(const std::shared_ptr<ast::INode>& action_node,
                                     std::string& table_name);

  std::vector<schema::Property> TranslateColumnDefinitions(
      const std::shared_ptr<ast::INode>& column_definition);
  schema::Property TranslateColumnDefinition(const std::shared_ptr<ast::INode>& node);
  void TranslateTableConstraint(const std::shared_ptr<ast::INode>& constraint_definition,
                                const std::string& table_name);

  void TranslateDropElement(const std::shared_ptr<ast::INode>& node, std::string& table_name);

  // DML statements

  void TranslateInsertStatement(const std::shared_ptr<ast::INode>& insert_stmt);
  void TranslateDeleteStatement(const std::shared_ptr<ast::INode>& delete_stmt);
  void TranslateUpdateStatement(const std::shared_ptr<ast::INode>& update_stmt);

  // Basic statements

  void TranslatePrimaryKey(const std::shared_ptr<ast::INode>& primary_key,
                           const std::string& constraint_name, const std::string& table_name);
  void TranslateForeignKey(const std::shared_ptr<ast::INode>& foreign_key,
                           const std::string& constraint_name, const std::string& table_name);

  std::string GetName(const std::shared_ptr<ast::INode>& name_node) const;
  std::string GetIdentifier(const std::shared_ptr<ast::INode>& node) const;

  void AddPropertyConstraints(const std::string& constraint_name_prefix, const std::string& label,
                              const std::string& property_name,
                              const std::vector<cypher::ConstraintType>& constraint_types);
  void AddRelationship(const std::string& relationship_type,
                       const std::string& start_label, const std::string& end_label,
                       const std::vector<schema::ApplyCondition>& apply_conditions);

  std::vector<schema::ApplyCondition> CreateApplyConditions(
      const std::string& start_label, const std::vector<std::string>& start_node_props,
      const std::string& end_label, const std::vector<std::string>& end_node_props
  );
  std::string CreateRelationshipType(const std::string& type_prefix);
  std::string CreatePKConstraintPrefix(const std::string& table_name) const;
  std::string CreateFKConstraintPrefix(const std::string& table_name,
                                       const std::string& ref_table_name) const;

  const schema::Node& GetNodeFromSchema(const std::string& label) const;

  // Validation

  bool HasChildren(const std::shared_ptr<ast::INode>& node, unsigned min_children_count = 1) const;
  bool IsCorrectStmtType(const std::shared_ptr<ast::INode>& node, ast::StmtType stmt_type) const;

  void ValidateHasChildren(const std::shared_ptr<ast::INode>& node, unsigned min_children_count = 1,
                           const std::string& details = "") const;
  void ValidateIsCorrectStmtType(const std::shared_ptr<ast::INode>& node,
                                 ast::StmtType stmt_type) const;
};

} // scc::translator
