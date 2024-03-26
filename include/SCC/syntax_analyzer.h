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

namespace scc::parser {

class SyntaxAnalyzer {
public:
  explicit SyntaxAnalyzer(std::deque<std::shared_ptr<ast::INode>>&& tokens);

  std::shared_ptr<ast::INode> Analyze();

private:
  std::deque<std::shared_ptr<ast::INode>> tokens_;

  // Start

  std::shared_ptr<ast::INode> General();
  ast::StmtType GetDLStType();
  std::shared_ptr<ast::INode> GetDL();

  // Define language (DDL | DML)

  ast::StmtType GetDDLStType();
  std::shared_ptr<ast::INode> GetDDLSt();

  ast::StmtType GetDMLStType();
  std::shared_ptr<ast::INode> GetDMLSt();

  // DDL Statements

  std::shared_ptr<ast::INode> GetCreateDatabaseSt();
  std::shared_ptr<ast::INode> GetCreateTableSt();
  std::shared_ptr<ast::INode> GetAlterTableSt();
  std::shared_ptr<ast::INode> GetDropDatabaseSt();
  std::shared_ptr<ast::INode> GetDropTableSt();

  // DDL Basic Statements

  // Gets tableDefinition, which consist
  // of tableConstraints and columnDefinitions
  std::shared_ptr<ast::INode> GetTableDefinition();
  std::shared_ptr<ast::INode> GetTableDefinitionObject();

  std::shared_ptr<ast::INode> GetColumnDefinition();
  std::shared_ptr<ast::INode> GetTableConstraint();

  // Gets listOf dropColumns and dropConstraints
  std::shared_ptr<ast::INode> GetDropListDefinition();
  std::shared_ptr<ast::INode> GetDropList();
  std::shared_ptr<ast::INode> GetDropObject();

  // DML Statements

  std::shared_ptr<ast::INode> GetInsertSt();
  std::shared_ptr<ast::INode> GetDeleteSt();
  std::shared_ptr<ast::INode> GetUpdateSt();

  // DML Basic Statements

  // Gets logical condition
  std::shared_ptr<ast::INode> GetCondition();
  std::shared_ptr<ast::INode> GetORCondition();
  std::shared_ptr<ast::INode> GetANDCondition();
  std::shared_ptr<ast::INode> GetNOTCondition();
  std::shared_ptr<ast::INode> GetPredicate();
  std::shared_ptr<ast::INode> GetExpression();

  // Gets Math expression like ((5 + 7) * (9 / 3) - (7^2 - 5 * 11))
  std::shared_ptr<ast::INode> GetMathExpression();
  std::shared_ptr<ast::INode> GetMathSum();
  std::shared_ptr<ast::INode> GetMathProduct();
  std::shared_ptr<ast::INode> GetMathPower();
  std::shared_ptr<ast::INode> GetMathValue();

  // Basic statements

  std::shared_ptr<ast::INode> GetDataType();
  std::shared_ptr<ast::INode> GetPrimaryKey();

  std::shared_ptr<ast::INode> GetForeignKey();
  std::shared_ptr<ast::INode> GetReference();

  std::shared_ptr<ast::INode> GetString();

  // Gets name like [ [ schema. ] db. ] table
  std::shared_ptr<ast::INode> GetName();
  std::shared_ptr<ast::INode> GetIdentifiers();

  std::shared_ptr<ast::INode> GetIdentifier();

  // Gets listOf arguments with certain type
  std::shared_ptr<ast::INode> GetListOf(
      ast::StmtType get_function_type);

  // Validation

  void ValidateNotEmpty() const;
  void ValidateIsWord(std::shared_ptr<ast::INode>& node) const;
  void ValidateIsOpeningRoundBracket(std::shared_ptr<ast::INode>& node) const;
  void ValidateIsClosingRoundBracket(std::shared_ptr<ast::INode>& node) const;
  void ValidateIsSingleQuote(std::shared_ptr<ast::INode>& node) const;
  void ValidateIsDoubleQuote(std::shared_ptr<ast::INode>& node) const;

  // Defining Node Datatype

  static bool IsBracket(std::shared_ptr<ast::INode>& node);
  static bool IsPunctuation(std::shared_ptr<ast::INode>& node);
  static bool IsWord(std::shared_ptr<ast::INode>& node);
  static bool IsNumber(std::shared_ptr<ast::INode>& node);
  static bool IsOperator(std::shared_ptr<ast::INode>& node);

  // Defining Node data

  static bool IsDot(std::shared_ptr<ast::INode>& node);
  static bool IsComma(std::shared_ptr<ast::INode>& node);
  static bool IsOpeningRoundBracket(std::shared_ptr<ast::INode>& node);
  static bool IsClosingRoundBracket(std::shared_ptr<ast::INode>& node);
  static bool IsSingleQuote(std::shared_ptr<ast::INode>& node);
  static bool IsDoubleQuote(std::shared_ptr<ast::INode>& node);
  static bool IsUnaryOperator(std::shared_ptr<ast::INode>& node);
  static bool IsBinaryOperator(std::shared_ptr<ast::INode>& node);
  static bool IsSemicolon(std::shared_ptr<ast::INode>& node);

  // Creates kinship between two nodes
  static void MakeKinship(std::shared_ptr<ast::INode>& parent,
                          std::shared_ptr<ast::INode>& child);

  // Work with deque of tokens

  std::shared_ptr<ast::INode>& peek_first_token() const;
  std::shared_ptr<ast::INode>& peek_last_token() const;
  std::shared_ptr<ast::INode> get_first_token();
  std::shared_ptr<ast::INode> get_last_token();
  void pop_first_token();
  void pop_last_token();
};

} // scc::parser
