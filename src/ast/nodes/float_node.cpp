#include "SCC/ast/nodes/float_node.h"

FloatNumNode::FloatNumNode(double value)
    : INode(DataType::kFloat), data(value) {}

void FloatNumNode::PrintData(std::ostream& stream) {
  stream << data;
}
void FloatNumNode::PrintType(std::ostream& stream) {
  stream << "FLOAT_NUMBER";
}
