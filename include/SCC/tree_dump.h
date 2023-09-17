#pragma once

#include <utility>

#include "SCC/ast.h"

#include "logger/log.hpp"

class TreeDump {
public:
  void DumpTree(const std::shared_ptr<Node>& AST);
  void PrintDumpInfo();
  void RecursiveTreeDump(const std::shared_ptr<Node>& node,
                         int parent_node_num);

  void MakeDumpPng();

  static std::string GetServiceNodeData(StatementType statement_type);

private:
  std::ofstream& dot_file_ = config.WriteTreeDump();
  std::string dot_file_path_ = config.get_tree_dump_path();
};
