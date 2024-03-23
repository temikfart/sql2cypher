#include "SCC/ast/nodes/int_node.h"

IntNumNode::IntNumNode(int value)
    : INode(DataType::kInt), data(value) {}

void IntNumNode::PrintData(std::ostream& stream) {
  stream << data;
}
void IntNumNode::PrintType(std::ostream& stream) {
  stream << "INT_NUMBER";
}
