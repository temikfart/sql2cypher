#pragma once

#include "SCC/ast.h"

class TreeDump {
public:
  static void Dump(const std::shared_ptr<Node> &AST);
  static void PrintDumpInfo(std::ofstream &dot_file);
  static void RecursiveTreeDump(const std::shared_ptr<Node> &node,
                         std::ofstream &dot_file, int parent_node_num);
  static std::string GetServiceNodeData(StatementType statement_type);
//  static void MakeDumpPng(const std::string &dot_file_path,
//                          const std::string &png_file_path);
};