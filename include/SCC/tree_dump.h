#pragma once

#include <utility>

#include "SCC/ast/ast.h"

#include "logger/log.hpp"

class TreeDump {
public:
  explicit TreeDump(const std::filesystem::path& out_path);

  void DumpTree(const std::shared_ptr<Node>& AST);
  void PrintDumpInfo();
  void RecursiveTreeDump(const std::shared_ptr<Node>& node,
                         int parent_node_num);

  void MakeDumpPng();

  static std::string GetServiceNodeData(StatementType statement_type);

private:
  std::ofstream dot_file_;
  std::string dot_file_path_;

  bool CloseTreeDumpFile();
};
