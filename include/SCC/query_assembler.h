#pragma once

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <memory>
#include <tuple>
#include <sstream>

#include "SCC/ast.h"
#include "SCC/config/scc_config.h"
#include "SCC/log.h"
#include "SCC/syntax_analyzer.h"

#include "logger/log.hpp"

// Column as property with standard data
using StdProperty = std::tuple<std::string, std::string>;

class QueryAssembler {
public:
  explicit QueryAssembler(const std::filesystem::path& out_path);

  bool CloseOutputFile();
  
  void Translate(std::shared_ptr<Node> AST);

private:
  std::shared_ptr<Node> ast_;
  std::ofstream out_;

  int constraint_counter = 0;
  int relationship_counter = 0;

  void TranslateProgram(std::shared_ptr<Node> node);
  void TranslateQuery(std::shared_ptr<Node> node);

  void TranslateDDLStatement(std::shared_ptr<Node> node);
  void TranslateDMLStatement(std::shared_ptr<Node> node);

  void TranslateCreateDatabase(std::shared_ptr<Node> node);
  void TranslateCreateTable(std::shared_ptr<Node> node);
  void TranslateAlterTable(std::shared_ptr<Node> node);
  void TranslateDropDatabase(std::shared_ptr<Node> node);
  void TranslateDropTable(std::shared_ptr<Node> node);

  void TranslateAlterTableActionAdd(std::shared_ptr<Node> action_node,
                                    std::string& table_name);
  void TranslateAlterTableActionDrop(std::shared_ptr<Node> action_node,
                                     std::string& table_name);

  std::vector<StdProperty> TranslateListOfColumnDefinitions(
      std::shared_ptr<Node> node);
  StdProperty TranslateColumnDefinition(std::shared_ptr<Node> node);

  void TranslateListOfTableConstraints(std::shared_ptr<Node> node,
                                       std::string& table_name);
  std::shared_ptr<Node> FindConstraint(std::shared_ptr<Node> node);

  void TranslateDropObject(std::shared_ptr<Node> node,
                           std::string& table_name);
  void TranslateListOfDropObjects(std::shared_ptr<Node> node,
                                  std::string& table_name);

  void TranslateInsert(std::shared_ptr<Node> node);
  void TranslateDelete(std::shared_ptr<Node> node);
  void TranslateUpdate(std::shared_ptr<Node> node);

  // Basic statements

  void TranslatePrimaryKey(
      std::shared_ptr<Node> key,
      std::string& constraint_name,
      std::string& table_name);
  void TranslateForeignKey(
      std::shared_ptr<Node> key,
      std::string& table_name);
  void CreateUniqueNodePropertyConstraint(
      const std::string& constraint_name,
      const std::string& LabelName,
      const std::vector<std::string>& properties);
  void CreateNodePropertyExistenceConstraint(
      const std::string& constraint_name,
      const std::string& LabelName,
      const std::string& property);
  void CreateRelationship(const std::string& label_name,
                          const std::string& ref_label_name);
  void RemoveProperties(const std::string& label_name,
                        const std::vector<std::string>& properties);

  std::vector<std::string> GetListOf(std::shared_ptr<Node> node,
                                     StatementType type);

  std::string TranslateName(std::shared_ptr<Node> node);
  std::string TranslateIdentifiers(std::shared_ptr<Node> node);
  std::string TranslateIdentifier(std::shared_ptr<Node> node);
};
