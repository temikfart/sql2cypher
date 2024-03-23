#pragma once

#include <iostream>
#include <memory>
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

  explicit INode(DataType data_type);
  virtual ~INode();

  void set_parent(std::shared_ptr<INode>& parent);
  std::shared_ptr<INode>& get_child(std::size_t node_num);
  std::size_t get_children_amount() const;

  void AddChild(std::shared_ptr<INode> const& node);
  static bool IsNodesEqual(const std::shared_ptr<INode>& node1,
                           const std::shared_ptr<INode>& node2);

  virtual void PrintData(std::ostream& stream) = 0;
  virtual void PrintType(std::ostream& stream) = 0;

protected:
  std::shared_ptr<INode> parent_ = nullptr;
  std::vector<std::shared_ptr<INode>> children_;

private:
  void ValidateChildNumber(std::size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<INode> const& node) const;
  virtual void ValidateType(DataType type) const = 0;
};

namespace Tree {
void PrintTreeRecursive(std::shared_ptr<INode> const& node,
                        std::ostream& stream);
};
