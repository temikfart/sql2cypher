#include "SCC/ast/nodes/char_node.h"

CharNode::CharNode(char ch, DataType data_type)
    : INode(data_type), data(ch) {}

std::ostream& CharNode::operator<<(std::ostream& os) {
  os << data;
  return os;
}
