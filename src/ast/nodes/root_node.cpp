#include "SCC/ast/nodes/root_node.h"

RootNode::RootNode() : INode(DataType::kRoot) {
  this->ValidateType(DataType::kRoot);
}

void RootNode::PrintData(std::ostream& stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream& stream) {
  stream << "ROOT";
}

void RootNode::ValidateType(DataType type) const {
  if (data_type != DataType::kRoot) {
    LOGE << "invalid type for RootNode: " << type;
    end(EXIT_FAILURE);
  }
}
