#pragma once

#include <utility>

#include "SCC/ast.h"

class TreeDump {
public:
  explicit TreeDump(std::string dot_path, std::string png_path);
  TreeDump() = default;

  void Dump(const std::shared_ptr<Node>& AST);
  void PrintDumpInfo();
  void RecursiveTreeDump(const std::shared_ptr<Node>& node,
                         int parent_node_num);

  void MakeDumpPng();

  static std::string GetServiceNodeData(StatementType statement_type);

private:
  std::string dot_path_ = "../resources/tree_dump/tree_dump.txt";
  std::string png_path_ = "../resources/tree_dump/tree_dump.png";

  std::ofstream dot_file_;

  void OpenDotFile();
  void CloseDotFile();
};
