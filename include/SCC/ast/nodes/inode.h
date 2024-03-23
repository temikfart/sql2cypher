#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "SCC/ast/data_types.h"
#include "SCC/ast/stmt_types.h"
#include "SCC/config/scc_config.h"

#include "logger/log.hpp"

class INode {
public:
  int line = 0;
  const DataType data_type = DataType::kNone;
  StmtType stmt_type = StmtType::kNone;

  std::shared_ptr<INode> parent = {};

  explicit INode(DataType data_type);
  virtual ~INode();

  std::shared_ptr<INode> get_child(std::size_t node_num);
  std::size_t get_children_amount() const;

  void AddChild(std::shared_ptr<INode> const& node);
  static bool IsNodesEqual(const std::shared_ptr<INode>& node1,
                           const std::shared_ptr<INode>& node2);

  virtual void PrintData(std::ostream& stream) = 0;
  virtual void PrintType(std::ostream& stream) = 0;

protected:
  std::vector<std::shared_ptr<INode>> children_ = {};
};
