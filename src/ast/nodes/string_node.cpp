#include "SCC/ast/nodes/string_node.h"

StringNode::StringNode(std::string string, DataType type)
    : INode(type), data(std::move(string)) {}

std::ostream& StringNode::operator<<(std::ostream& os) {
  os << data;
  return os;
}
