#pragma once

#include "SCC/log.h"
#include "SCC/config.h"
#include "SCC/ast.h"
#include "SCC/syntax_analyzer.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <tuple>

// Column as property with standard data
using StdProperty = std::tuple<std::string, std::string>;

class QueryAssembler {
public:
  void Translate(std::shared_ptr<Node> AST);

private:
  std::shared_ptr<Node> ast_;
  std::ofstream &out_ = config.WriteCypher();
  int constraint_counter = 0;

  void TranslateProgram(std::shared_ptr<Node> node);
  void TranslateQuery(std::shared_ptr<Node> node);

  void TranslateDDLStatement(std::shared_ptr<Node> node);
  void TranslateDMLStatement(std::shared_ptr<Node> node);

  void TranslateCreateDatabase(std::shared_ptr<Node> node);
  void TranslateCreateTable(std::shared_ptr<Node> node);
  void TranslateAlterTable(std::shared_ptr<Node> node);
  void TranslateDropDatabase(std::shared_ptr<Node> node);
  void TranslateDropTable(std::shared_ptr<Node> node);

  std::vector<StdProperty> TranslateListOfColumnDefinitions(
      std::shared_ptr<Node> node);
  StdProperty TranslateColumnDefinition(std::shared_ptr<Node> node);

  void TranslateTableConstraint(std::shared_ptr<Node> node,
                                std::string table_name);
  std::shared_ptr<Node> FindConstraint(std::shared_ptr<Node> node);

  void TranslateDropObject(std::shared_ptr<Node> node,
                           std::string table_name);
  void TranslateListOfDropObjects(std::shared_ptr<Node> node,
                                  std::string table_name);

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
      std::string& constraint_name,
      std::string& table_name);
  void CreateUniqueNodePropertyConstraint(
      const std::string& constraint_name,
      const std::string& LabelName,
      const std::vector<std::string>& properties);
  void CreateNodePropertyExistenceConstraint(
      const std::string& constraint_name,
      const std::string& LabelName,
      const std::string& property);

  std::vector<std::string> GetListOfProperties(std::shared_ptr<Node> node);
  std::vector<std::string> GetListOfNames(std::shared_ptr<Node> node);

  std::string TranslateName(std::shared_ptr<Node> node);
  std::string TranslateIdentifiers(std::shared_ptr<Node> node);
  std::string TranslateIdentifier(std::shared_ptr<Node> node);
};
