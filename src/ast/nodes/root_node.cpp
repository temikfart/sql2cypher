#include "SCC/ast/nodes/root_node.h"

RootNode::RootNode() : INode(DataType::ROOT) {
  this->ValidateType(DataType::ROOT);
}

void RootNode::PrintData(std::ostream& stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream& stream) {
  stream << "ROOT";
}

void RootNode::ValidateType(DataType type) const {
  if (type != DataType::ROOT) {
    LOGE << "invalid type for RootNode: " << type;
    end(EXIT_FAILURE);
  }
}
