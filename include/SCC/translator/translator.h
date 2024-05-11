#pragma once

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <memory>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "SCC/ast/nodes/inode.h"
#include "SCC/config/scc_config.h"
#include "SCC/log/log.h"
#include "SCC/parser/parser.h"
#include "SCC/translator/cypher/clauses/create.h"
#include "SCC/translator/cypher/clauses/delete.h"
#include "SCC/translator/cypher/clauses/drop.h"
#include "SCC/translator/cypher/clauses/set.h"

#include "logger/log.hpp"

namespace scc::translator {

class translation_error : public std::logic_error {
public:
  explicit translation_error(const std::string& message);
};

// Column as property with standard data
using StdProperty = std::tuple<std::string, std::string>;

class Translator {
public:
  explicit Translator(std::shared_ptr<ast::INode> ast, const std::filesystem::path& out_path);

  void Translate();

private:
  std::shared_ptr<ast::INode> ast_;
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

  std::vector<cypher::NodeProperty> TranslateColumnDefinitions(
      const std::shared_ptr<ast::INode>& column_definition);
  cypher::NodeProperty TranslateColumnDefinition(const std::shared_ptr<ast::INode>& node);
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

  void CreateConstraint(const std::string& constraint_name,
                        const std::string& label_name,
                        const std::string& property_name,
                        cypher::ConstraintType constraint_type);
  void CreateRelationship(const std::string& relationship_type, const std::string& start_label_name,
                          const std::string& end_label_name);
  void RemoveProperty(const std::string& label_name, const std::string& property_name);
  std::string CreateRelationshipType(const std::string& type_prefix);
  std::string CreatePrimaryKeyConstraintName(const std::string& table_name) const;
  std::string CreateForeignKeyConstraintName(const std::string& table_name,
                                             const std::string& ref_table_name) const;

  // Validation

  bool HasChildren(const std::shared_ptr<ast::INode>& node, unsigned min_children_count = 1) const;
  bool IsCorrectStmtType(const std::shared_ptr<ast::INode>& node, ast::StmtType stmt_type) const;

  void ValidateHasChildren(const std::shared_ptr<ast::INode>& node, unsigned min_children_count = 1,
                           const std::string& details = "") const;
  void ValidateIsCorrectStmtType(const std::shared_ptr<ast::INode>& node,
                                 ast::StmtType stmt_type) const;
};

} // scc::translator
