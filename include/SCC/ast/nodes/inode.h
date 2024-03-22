#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "SCC/ast/data_types.h"
#include "SCC/ast/statement_types.h"
#include "SCC/config/scc_config.h"

#include "logger/log.hpp"

class INode {
public:
  explicit INode(DataType type);
  virtual ~INode();

  void set_line(int line);
  int get_line();
  void set_st_type(StatementType type);
  StatementType get_st_type();
  void set_parent(std::shared_ptr<INode>& parent);
  DataType get_type() const;
  std::shared_ptr<INode>& get_child(size_t node_num);
  size_t get_children_amount() const;

  void AddChild(std::shared_ptr<INode> const& node);
  static bool IsNodesEqual(const std::shared_ptr<INode>& node1,
                           const std::shared_ptr<INode>& node2);

  virtual void PrintData(std::ostream& stream) = 0;
  virtual void PrintType(std::ostream& stream) = 0;

protected:
  int line_ = 0;
  DataType type_;
  StatementType st_type_ = StatementType::EMPTY_TYPE;
  std::shared_ptr<INode> parent_ = nullptr;
  std::vector<std::shared_ptr<INode>> children_;

private:
  void ValidateChildNumber(size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<INode> const& node) const;
  void ValidateStType(StatementType type);
  virtual void ValidateType(DataType type) const = 0;
};

namespace Tree {
void PrintTreeRecursive(std::shared_ptr<INode> const& node,
                        std::ostream& stream);
};
