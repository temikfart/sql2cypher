#include "SCC/ast/nodes/root_node.h"

RootNode::RootNode() : INode(DataType::kRoot) {}

std::ostream& RootNode::operator<<(std::ostream& os) {
  os << "root";
  return os;
}
