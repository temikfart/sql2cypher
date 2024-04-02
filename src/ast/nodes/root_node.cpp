#include "SCC/ast/nodes/root_node.h"

namespace scc::ast {

RootNode::RootNode() : INode(DataType::kRoot) {}
RootNode::RootNode(StmtType stmt_type) : INode(DataType::kRoot, stmt_type) {}

std::ostream& RootNode::operator<<(std::ostream& os) {
  os << "root";
  return os;
}

} // scc::ast
