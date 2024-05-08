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
#include "SCC/common/string_utils.h"
#include "SCC/config/scc_config.h"
#include "SCC/log/log.h"
#include "SCC/parser/common/base_stmttype_classifier.h"
#include "SCC/parser/common/node_data_classifier.h"
#include "SCC/parser/common/node_datatype_classifier.h"

#include "logger/log.hpp"

namespace scc::parser {

class parsing_error : public std::logic_error {
public:
  explicit parsing_error(const std::string& message);
};

class Parser {
public:
  explicit Parser(std::deque<std::shared_ptr<ast::INode>>&& tokens);

  std::shared_ptr<ast::INode> Parse();

private:
  std::deque<std::shared_ptr<ast::INode>> tokens_;

  const std::shared_ptr<ast::INode>& PeekToken(unsigned idx = 0) const;
  std::shared_ptr<ast::INode> NextToken();

  std::shared_ptr<ast::INode> ParseQuery();
  std::shared_ptr<ast::INode> ParseBaseStatement();

  ast::StmtType ParseDDLStatementType();
  ast::StmtType ParseDMLStatementType();
  std::shared_ptr<ast::INode> ParseDDLStatement();
  std::shared_ptr<ast::INode> ParseDMLStatement();

  std::shared_ptr<ast::INode> ParseCreateDatabaseStatement();
  std::shared_ptr<ast::INode> ParseCreateTableStatement();
  std::shared_ptr<ast::INode> ParseAlterTableStatement();
  std::shared_ptr<ast::INode> ParseDropDatabaseStatement();
  std::shared_ptr<ast::INode> ParseDropTableStatement();

  std::shared_ptr<ast::INode> ParseTableDefinition();
  std::shared_ptr<ast::INode> ParseTableDefinitionElement();
  ast::StmtType ParseTableDefinitionElementType();
  std::shared_ptr<ast::INode> ParseColumnDefinition();
  std::shared_ptr<ast::INode> ParseTableConstraint(ast::StmtType stmt_type = ast::StmtType::kNone);

  std::shared_ptr<ast::INode> ParseAlterAddListDefinition();
  std::shared_ptr<ast::INode> ParseAlterAddElement();
  std::shared_ptr<ast::INode> ParseAlterDropListDefinition();
  std::shared_ptr<ast::INode> ParseAlterDropElement();
  void ParseAlterDropColumns(std::shared_ptr<ast::INode>& parent);

  std::shared_ptr<ast::INode> ParseInsertStatement();
  std::shared_ptr<ast::INode> ParseDeleteStatement();
  std::shared_ptr<ast::INode> ParseUpdateStatement();

  std::shared_ptr<ast::INode> ParseCondition();
  std::shared_ptr<ast::INode> ParseORCondition();
  std::shared_ptr<ast::INode> ParseANDCondition();
  std::shared_ptr<ast::INode> ParseNOTCondition();
  std::shared_ptr<ast::INode> ParsePredicate();
  std::shared_ptr<ast::INode> ParseExpression();

  std::shared_ptr<ast::INode> ParseMathExpression();
  std::shared_ptr<ast::INode> ParseMathSum();
  std::shared_ptr<ast::INode> ParseMathProduct();
  std::shared_ptr<ast::INode> ParseMathPower();
  std::shared_ptr<ast::INode> ParseMathValue();

  ast::StmtType DetermineConstraintType(const std::string& keyword) const;
  ast::StmtType DetermineAlterTableActionType(const std::string& keyword) const;
  ast::StmtType DetermineDropElementType(const std::string& keyword) const;
  ast::StmtType DetermineLogicalOperatorType(const std::string& keyword) const;

  bool DetermineIsFullConstraintDefinition(const std::string& keyword) const;
  bool DetermineIsPrimaryKey(const std::string& keyword) const;
  bool DetermineIsForeignKey(const std::string& keyword) const;
  bool DetermineIsReferences(const std::string& keyword) const;
  bool DetermineIsOROperator(const std::string& keyword) const;
  bool DetermineIsANDOperator(const std::string& keyword) const;
  bool DetermineIsNOTOperator(const std::string& keyword) const;
  bool DetermineIsPrefix(const std::string_view& string, const std::string& prefix) const;

  std::shared_ptr<ast::INode> ParseListOf(ast::StmtType element_type);
  std::shared_ptr<ast::INode> ParsePrimaryKey();
  std::shared_ptr<ast::INode> ParseForeignKey();
  std::shared_ptr<ast::INode> ParseReference();

  std::shared_ptr<ast::INode> ParseDatabaseName();
  std::shared_ptr<ast::INode> ParseTableName();
  std::shared_ptr<ast::INode> ParseColumnName();

  std::shared_ptr<ast::INode> ParseDataType();
  std::shared_ptr<ast::INode> ParseString();
  std::shared_ptr<ast::INode> ParseName();
  std::shared_ptr<ast::INode> ParseIdentifier();

  bool HasTokens(unsigned min_count = 1) const;
  void ValidateHasTokens(const std::string& details = "") const;
  void ValidateHasTokens(unsigned min_count = 1) const;
  void ValidateHasNotTokens() const;
  void ValidateIsWord(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsOpeningRoundBracket(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsClosingRoundBracket(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsSingleQuote(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsDoubleQuote(const std::shared_ptr<ast::INode>& node) const;
  void ValidateIsBinaryOperator(const std::shared_ptr<ast::INode>& node) const;
};

} // scc::parser
