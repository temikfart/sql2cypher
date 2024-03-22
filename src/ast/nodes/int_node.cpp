#include "SCC/ast/nodes/int_node.h"

IntNumNode::IntNumNode(int value, DataType type)
    : INode(type), data_(value) {
  this->ValidateType(type);
}

int IntNumNode::get_data() const {
  return data_;
}

void IntNumNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void IntNumNode::PrintType(std::ostream& stream) {
  stream << "INT_NUMBER";
}

void IntNumNode::ValidateType(DataType type) const {
  if (type != DataType::INT_NUMBER) {
    LOGE << "invalid type for IntNumNode: " << type;
    end(EXIT_FAILURE);
  }
}
