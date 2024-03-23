#include "SCC/ast/nodes/root_node.h"

RootNode::RootNode() : INode(DataType::kRoot) {}

void RootNode::PrintData(std::ostream& stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream& stream) {
  stream << "ROOT";
}
