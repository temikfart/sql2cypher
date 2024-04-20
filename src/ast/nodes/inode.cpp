#include "SCC/ast/nodes/inode.h"

namespace scc::ast {

INode::INode(DataType data_type) : data_type(data_type) {}
INode::INode(DataType data_type, StmtType stmt_type)
    : data_type(data_type), stmt_type(stmt_type) {}
INode::INode(DataType data_type, StmtType stmt_type, int line)
    : line(line), data_type(data_type), stmt_type(stmt_type) {}
INode::~INode() = default;

std::shared_ptr<INode> INode::get_child(std::size_t node_num) {
  if (node_num >= children_.size())
    return {};
  return children_[node_num];
}
std::size_t INode::ChildrenCount() const {
  return children_.size();
}

void INode::AddChild(std::shared_ptr<INode> const& node) {
  children_.push_back(node);
}

} // scc::ast
