#pragma once

#include <utility>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"

#include "logger/log.hpp"

class TreeDump {
public:
  explicit TreeDump(const std::filesystem::path& out_path);

  void DumpTree(const std::shared_ptr<INode>& AST);
  void PrintDumpInfo();
  void RecursiveTreeDump(const std::shared_ptr<INode>& node,
                         int parent_node_num);

  void MakeDumpPng();

  static std::string GetServiceNodeData(StatementType statement_type);

private:
  std::ofstream dot_file_;
  std::string dot_file_path_;

  bool CloseTreeDumpFile();
};
