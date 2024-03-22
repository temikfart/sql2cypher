#include "SCC/ast/nodes/service_node.h"

ServiceNode::ServiceNode() : INode(DataType::SERVICE) {
  this->ValidateType(DataType::SERVICE);
}

void ServiceNode::PrintData(std::ostream& stream) {
  stream << "SERVICE";
}
void ServiceNode::PrintType(std::ostream& stream) {
  stream << "SERVICE";
}

void ServiceNode::ValidateType(DataType type) const {
  if (type != DataType::SERVICE) {
    LOGE << "invalid type for ServiceNode: " << type;
    end(EXIT_FAILURE);
  }
}
