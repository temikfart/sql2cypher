#include "SCC/ast/nodes/int_node.h"

IntNumNode::IntNumNode(int value)
    : INode(DataType::kInt), data(value) {}

std::ostream& IntNumNode::operator<<(std::ostream& os) {
  os << data;
  return os;
}
