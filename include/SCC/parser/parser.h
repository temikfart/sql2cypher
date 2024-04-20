#pragma once

#include <deque>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "SCC/ast/common/utils.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/config/scc_config.h"
#include "SCC/log/log.h"
#include "SCC/parser/common/base_stmttype_classifier.h"
#include "SCC/parser/common/node_data_classifier.h"
#include "SCC/parser/common/node_datatype_classifier.h"

#include "logger/log.hpp"

namespace scc::parser {

class parsing_error : private std::logic_error {
public:
  explicit parsing_error(const std::string& message);
};

class Parser {
public:
  explicit Parser(std::deque<std::shared_ptr<ast::INode>>&& tokens);

  std::shared_ptr<ast::INode> Parse();

private:
  std::deque<std::shared_ptr<ast::INode>> tokens_;

  // Work with deque of tokens

  const std::shared_ptr<ast::INode>& PeekToken() const;
  std::shared_ptr<ast::INode> NextToken();

  // Start

  std::shared_ptr<ast::INode> ParseQuery();
  std::shared_ptr<ast::INode> ParseBaseStatement();
  std::shared_ptr<ast::INode> ParseNextQueries();

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

  void ValidateHasTokens(const std::string& details = "") const;
  void ValidateHasNotTokens() const;
  void ValidateIsWord(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsOpeningRoundBracket(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsClosingRoundBracket(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsSingleQuote(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsDoubleQuote(const std::shared_ptr<ast::INode>& node) const;
};

} // scc::parser
