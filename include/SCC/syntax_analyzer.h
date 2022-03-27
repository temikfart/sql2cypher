#pragma once

#include <iostream>
#include <memory>
#include <deque>
#include <utility>
#include "SCC/config.h"
#include "SCC/log.h"
#include "SCC/ast.h"

enum StatementType {
  EMPTY_TYPE,
  Program,
  query,
  ddlStatement,
  dmlStatement,
//  alterTableStatement,
//  createDatabaseStatement,
//  createTableStatement,
//  dropDatabaseStatement,
//  dropTableStatement,
//  deleteStatement,
//  insertStatement,
//  updateStatement
  StTypeCount
};

class SyntaxAnalyzer {
  using NodePtr = std::shared_ptr<Node>;
public:
  NodePtr Analyze(std::deque<NodePtr> tokens_array) {
    tokens_array_ = std::move(tokens_array);
    
    return this->General();
  }

private:
  std::deque<NodePtr> tokens_array_;
  
  /* Grammar rules */
  // TODO: fix AST implementation. Can't return nothing except Node type
//  std::shared_ptr<Node> General() {
//    std::shared_ptr<IntNumNode> root = std::make_shared<IntNumNode>(5);
//
//    root->set_st_type(StatementType::query);
//
//    return root;
//  }
//  NodePtr GetDL() {
//    std::
//  }
};
