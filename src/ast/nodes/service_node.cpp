#include "SCC/ast/nodes/service_node.h"

ServiceNode::ServiceNode() : INode(DataType::kService) {}

std::ostream& ServiceNode::operator<<(std::ostream& os) {
  os << "service";
  return os;
}
