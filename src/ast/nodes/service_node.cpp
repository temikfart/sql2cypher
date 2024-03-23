#include "SCC/ast/nodes/service_node.h"

ServiceNode::ServiceNode() : INode(DataType::kService) {}

void ServiceNode::PrintData(std::ostream& stream) {
  stream << "SERVICE";
}
void ServiceNode::PrintType(std::ostream& stream) {
  stream << "SERVICE";
}
