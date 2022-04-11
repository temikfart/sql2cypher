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

  // Start

  std::shared_ptr<Node> General();
  std::shared_ptr<Node> GetDL();

  // Define language (DDL | DML)

  std::shared_ptr<Node> GetDDLSt();
  std::shared_ptr<Node> GetDMLSt();

  // DDL Statements

  std::shared_ptr<Node> GetCreateDatabaseSt();
  std::shared_ptr<Node> GetCreateTableSt();
  std::shared_ptr<Node> GetAlterTableSt();
  std::shared_ptr<Node> GetDropDatabaseSt();
  std::shared_ptr<Node> GetDropTableSt();

  // DDL Basic Statements

  // Gets tableDefinition, which consist
  // of tableConstraints and columnDefinitions
  std::shared_ptr<Node> GetTableDefinition();
  std::shared_ptr<Node> GetTableDefinitionList();

  std::shared_ptr<Node> GetColumnDefinition();
  std::shared_ptr<Node> GetTableConstraint();

  // Gets listOf dropColumns and dropConstraints
  std::shared_ptr<Node> GetDropListDefinition();
  std::shared_ptr<Node> GetDropList();
  std::shared_ptr<Node> GetDropObject();

  // DML Statements

  std::shared_ptr<Node> GetInsertSt();
  std::shared_ptr<Node> GetDeleteSt();
  std::shared_ptr<Node> GetUpdateSt();

  // DML Basic Statements

  // Gets logical condition
  std::shared_ptr<Node> GetCondition();
  std::shared_ptr<Node> GetORCondition();
  std::shared_ptr<Node> GetANDCondition();
  std::shared_ptr<Node> GetNOTCondition();
  std::shared_ptr<Node> GetPredicate();
  std::shared_ptr<Node> GetExpression();

  // Gets Math expression like ((5 + 7) * (9 / 3) - (7^2 - 5 * 11))
  std::shared_ptr<Node> GetMathExpression();
  std::shared_ptr<Node> GetMathSum();
  std::shared_ptr<Node> GetMathProduct();
  std::shared_ptr<Node> GetMathPower();
  std::shared_ptr<Node> GetMathValue();

  // Basic statements

  std::shared_ptr<Node> GetDataType();
  std::shared_ptr<Node> GetPrimaryKey();

  std::shared_ptr<Node> GetForeignKey();
  std::shared_ptr<Node> GetReference();

  std::shared_ptr<Node> GetString();

  // Gets name like [ [ schema. ] db. ] table
  std::shared_ptr<Node> GetName();
  std::shared_ptr<Node> GetIdentifiers();

  std::shared_ptr<Node> GetIdentifier();

  // Gets listOf arguments with certain type
  std::shared_ptr<Node> GetListOf(
      StatementType get_function_type);

  // Validation

  void ValidateNotEmpty() const;
  void ValidateIsWord(std::shared_ptr<Node> &node) const;
  void ValidateIsOpeningRoundBracket(std::shared_ptr<Node> &node) const;
  void ValidateIsClosingRoundBracket(std::shared_ptr<Node> &node) const;
  void ValidateIsSingleQuote(std::shared_ptr<Node> &node) const;
  void ValidateIsDoubleQuote(std::shared_ptr<Node> &node) const;

  // Defining Node Datatype

  static bool IsBracket(std::shared_ptr<Node> &node);
  static bool IsPunctuation(std::shared_ptr<Node> &node);
  static bool IsWord(std::shared_ptr<Node> &node);
  static bool IsNumber(std::shared_ptr<Node> &node);
  static bool IsOperator(std::shared_ptr<Node> &node);

  // Defining Node data

  static bool IsDot(std::shared_ptr<Node> &node);
  static bool IsComma(std::shared_ptr<Node> &node);
  static bool IsOpeningRoundBracket(std::shared_ptr<Node> &node);
  static bool IsClosingRoundBracket(std::shared_ptr<Node> &node);
  static bool IsSingleQuote(std::shared_ptr<Node> &node);
  static bool IsDoubleQuote(std::shared_ptr<Node> &node);
  static bool IsUnaryOperator(std::shared_ptr<Node> &node);
  static bool IsBinaryOperator(std::shared_ptr<Node> &node);
  static bool IsSemicolon(std::shared_ptr<Node> &node);

  // Creates kinship between two nodes
  static void MakeKinship(std::shared_ptr<Node>& parent,
                          std::shared_ptr<Node>& child);

  // Work with deque of tokens

  std::shared_ptr<Node>& peek_first_token() const;
  std::shared_ptr<Node>& peek_last_token() const;
  std::shared_ptr<Node> get_first_token();
  std::shared_ptr<Node> get_last_token();
  void pop_first_token();
  void pop_last_token();
};
