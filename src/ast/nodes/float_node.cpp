#include "SCC/ast/nodes/float_node.h"

namespace scc::ast {

FloatNumNode::FloatNumNode(double value)
    : INode(DataType::kFloat), data(value) {}

} // scc::ast
