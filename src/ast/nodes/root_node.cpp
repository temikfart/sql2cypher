#include "SCC/ast/nodes/root_node.h"

namespace scc::ast {

RootNode::RootNode() : INode(DataType::kRoot) {}

std::ostream& RootNode::operator<<(std::ostream& os) {
  os << "root";
  return os;
}

} // scc::ast
