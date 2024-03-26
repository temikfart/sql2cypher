#include "SCC/ast/nodes/service_node.h"

namespace scc::ast {

ServiceNode::ServiceNode() : INode(DataType::kService) {}

std::ostream& ServiceNode::operator<<(std::ostream& os) {
  os << "service";
  return os;
}

} // scc::ast
