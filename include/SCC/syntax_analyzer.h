#pragma once

#include <iostream>
#include <memory>
#include <deque>
#include <utility>
#include "SCC/config.h"
#include "SCC/log.h"
#include "SCC/ast.h"

enum SQLDataType {
  SQL_int,
  SQL_float,
  SQL_char,
  SQL_varchar
};

class SyntaxAnalyzer {
public:
  std::shared_ptr<Node> Analyze(
      std::deque<std::shared_ptr<Node>> tokens_array);

private:
  std::deque<std::shared_ptr<Node>> tokens_array_;

  /* Grammar rules */
  std::shared_ptr<Node> General();
  std::shared_ptr<Node> GetDL();
  std::shared_ptr<Node> GetDDLSt();
  std::shared_ptr<Node> GetDMLSt();

  std::shared_ptr<Node> GetCreateDatabaseSt();
  std::shared_ptr<Node> GetCreateTableSt();
  std::shared_ptr<Node> GetAlterTableSt();
  std::shared_ptr<Node> GetDropDatabaseSt();
  std::shared_ptr<Node> GetDropTableSt();

  std::shared_ptr<Node> GetInsertSt();
  std::shared_ptr<Node> GetDeleteSt();
  std::shared_ptr<Node> GetUpdateSt();

  std::shared_ptr<Node> GetColumnDefinition();
  std::shared_ptr<Node> GetDataType();
  std::shared_ptr<Node> GetTableConstraint();
  std::shared_ptr<Node> GetPrimaryKey();
  std::shared_ptr<Node> GetForeignKey();
  std::shared_ptr<Node> GetCondition();
  std::shared_ptr<Node> GetORCondition();
  std::shared_ptr<Node> GetANDCondition();
  std::shared_ptr<Node> GetNOTCondition();
  std::shared_ptr<Node> GetPredicate();
  std::shared_ptr<Node> GetExpression();
  std::shared_ptr<Node> GetName();

  void ValidateIsFirstWord(std::shared_ptr<Node>& node) const;
};
