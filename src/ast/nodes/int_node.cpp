#include "SCC/ast/nodes/int_node.h"

namespace scc::ast {

IntNumNode::IntNumNode(int value)
    : INode(DataType::kInt), data(value) {}

} // scc::ast
