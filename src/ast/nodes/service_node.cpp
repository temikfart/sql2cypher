#include "SCC/ast/nodes/service_node.h"

namespace scc::ast {

ServiceNode::ServiceNode() : INode(DataType::kService) {}
ServiceNode::ServiceNode(StmtType stmt_type) : INode(DataType::kService, stmt_type) {}

std::ostream& ServiceNode::operator<<(std::ostream& os) {
  os << "service";
  return os;
}

} // scc::ast
