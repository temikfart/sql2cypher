#include "SCC/ast/nodes/root_node.h"

namespace scc::ast {

RootNode::RootNode() : INode(DataType::kRoot) {}
RootNode::RootNode(StmtType stmt_type) : INode(DataType::kRoot, stmt_type) {}

} // scc::ast
