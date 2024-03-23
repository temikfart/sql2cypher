#include "SCC/ast/nodes/float_node.h"

FloatNumNode::FloatNumNode(double value)
    : INode(DataType::kFloat), data(value) {}

std::ostream& FloatNumNode::operator<<(std::ostream& os) {
  os << data;
  return os;
}
