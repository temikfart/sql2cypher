#pragma once

#include <deque>
#include <iostream>
#include <memory>
#include <utility>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/config/scc_config.h"
#include "SCC/log.h"

#include "logger/log.hpp"

class SyntaxAnalyzer {
public:
  std::shared_ptr<INode> Analyze(
      std::deque<std::shared_ptr<INode>> tokens_array);

private:
  std::deque<std::shared_ptr<INode>> tokens_array_;

  // Start

  std::shared_ptr<INode> General();
  StmtType GetDLStType();
  std::shared_ptr<INode> GetDL();

  // Define language (DDL | DML)

  StmtType GetDDLStType();
  std::shared_ptr<INode> GetDDLSt();

  StmtType GetDMLStType();
  std::shared_ptr<INode> GetDMLSt();

  // DDL Statements

  std::shared_ptr<INode> GetCreateDatabaseSt();
  std::shared_ptr<INode> GetCreateTableSt();
  std::shared_ptr<INode> GetAlterTableSt();
  std::shared_ptr<INode> GetDropDatabaseSt();
  std::shared_ptr<INode> GetDropTableSt();

  // DDL Basic Statements

  // Gets tableDefinition, which consist
  // of tableConstraints and columnDefinitions
  std::shared_ptr<INode> GetTableDefinition();
  std::shared_ptr<INode> GetTableDefinitionObject();

  std::shared_ptr<INode> GetColumnDefinition();
  std::shared_ptr<INode> GetTableConstraint();

  // Gets listOf dropColumns and dropConstraints
  std::shared_ptr<INode> GetDropListDefinition();
  std::shared_ptr<INode> GetDropList();
  std::shared_ptr<INode> GetDropObject();

  // DML Statements

  std::shared_ptr<INode> GetInsertSt();
  std::shared_ptr<INode> GetDeleteSt();
  std::shared_ptr<INode> GetUpdateSt();

  // DML Basic Statements

  // Gets logical condition
  std::shared_ptr<INode> GetCondition();
  std::shared_ptr<INode> GetORCondition();
  std::shared_ptr<INode> GetANDCondition();
  std::shared_ptr<INode> GetNOTCondition();
  std::shared_ptr<INode> GetPredicate();
  std::shared_ptr<INode> GetExpression();

  // Gets Math expression like ((5 + 7) * (9 / 3) - (7^2 - 5 * 11))
  std::shared_ptr<INode> GetMathExpression();
  std::shared_ptr<INode> GetMathSum();
  std::shared_ptr<INode> GetMathProduct();
  std::shared_ptr<INode> GetMathPower();
  std::shared_ptr<INode> GetMathValue();

  // Basic statements

  std::shared_ptr<INode> GetDataType();
  std::shared_ptr<INode> GetPrimaryKey();

  std::shared_ptr<INode> GetForeignKey();
  std::shared_ptr<INode> GetReference();

  std::shared_ptr<INode> GetString();

  // Gets name like [ [ schema. ] db. ] table
  std::shared_ptr<INode> GetName();
  std::shared_ptr<INode> GetIdentifiers();

  std::shared_ptr<INode> GetIdentifier();

  // Gets listOf arguments with certain type
  std::shared_ptr<INode> GetListOf(
      StmtType get_function_type);

  // Validation

  void ValidateNotEmpty() const;
  void ValidateIsWord(std::shared_ptr<INode>& node) const;
  void ValidateIsOpeningRoundBracket(std::shared_ptr<INode>& node) const;
  void ValidateIsClosingRoundBracket(std::shared_ptr<INode>& node) const;
  void ValidateIsSingleQuote(std::shared_ptr<INode>& node) const;
  void ValidateIsDoubleQuote(std::shared_ptr<INode>& node) const;

  // Defining Node Datatype

  static bool IsBracket(std::shared_ptr<INode>& node);
  static bool IsPunctuation(std::shared_ptr<INode>& node);
  static bool IsWord(std::shared_ptr<INode>& node);
  static bool IsNumber(std::shared_ptr<INode>& node);
  static bool IsOperator(std::shared_ptr<INode>& node);

  // Defining Node data

  static bool IsDot(std::shared_ptr<INode>& node);
  static bool IsComma(std::shared_ptr<INode>& node);
  static bool IsOpeningRoundBracket(std::shared_ptr<INode>& node);
  static bool IsClosingRoundBracket(std::shared_ptr<INode>& node);
  static bool IsSingleQuote(std::shared_ptr<INode>& node);
  static bool IsDoubleQuote(std::shared_ptr<INode>& node);
  static bool IsUnaryOperator(std::shared_ptr<INode>& node);
  static bool IsBinaryOperator(std::shared_ptr<INode>& node);
  static bool IsSemicolon(std::shared_ptr<INode>& node);

  // Creates kinship between two nodes
  static void MakeKinship(std::shared_ptr<INode>& parent,
                          std::shared_ptr<INode>& child);

  // Work with deque of tokens

  std::shared_ptr<INode>& peek_first_token() const;
  std::shared_ptr<INode>& peek_last_token() const;
  std::shared_ptr<INode> get_first_token();
  std::shared_ptr<INode> get_last_token();
  void pop_first_token();
  void pop_last_token();
};
