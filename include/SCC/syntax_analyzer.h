#pragma once

#include <iostream>
#include <memory>
#include <deque>
#include <utility>
#include "SCC/config.h"
#include "SCC/log.h"
#include "SCC/ast.h"

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

  std::shared_ptr<Node> GetTableDefinition();
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
  std::shared_ptr<Node> GetIdentifier();
  std::shared_ptr<Node> GetIdentifiers();

  template <typename GetFunc>
  std::shared_ptr<Node> GetListOf(GetFunc GetArg);

  void ValidateIsWord(std::shared_ptr<Node> &node) const;
  void ValidateIsRoundBracket(std::shared_ptr<Node> &node) const;

  static bool IsPunctuation(std::shared_ptr<Node> &node);

  static bool IsDot(std::shared_ptr<Node> &node);
  static bool IsRoundBracket(std::shared_ptr<Node> &node);
};
