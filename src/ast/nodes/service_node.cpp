#include "SCC/ast/nodes/service_node.h"

ServiceNode::ServiceNode() : INode(DataType::kService) {
  this->ValidateType(DataType::kService);
}

void ServiceNode::PrintData(std::ostream& stream) {
  stream << "SERVICE";
}
void ServiceNode::PrintType(std::ostream& stream) {
  stream << "SERVICE";
}

void ServiceNode::ValidateType(DataType type) const {
  if (data_type != DataType::kService) {
    LOGE << "invalid type for ServiceNode: " << type;
    end(EXIT_FAILURE);
  }
}
