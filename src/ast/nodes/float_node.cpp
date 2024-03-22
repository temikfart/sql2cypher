#include "SCC/ast/nodes/float_node.h"

FloatNumNode::FloatNumNode(double value, DataType type)
    : INode(type), data_(value) {
  this->ValidateType(type);
}

double FloatNumNode::get_data() const {
  return data_;
}

void FloatNumNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void FloatNumNode::PrintType(std::ostream& stream) {
  stream << "FLOAT_NUMBER";
}

void FloatNumNode::ValidateType(DataType type) const {
  if (type != DataType::FLOAT_NUMBER) {
    LOGE << "invalid type for FloatNumNode: " << type;
    end(EXIT_FAILURE);
  }
}