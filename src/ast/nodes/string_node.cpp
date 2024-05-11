#include "SCC/ast/nodes/string_node.h"

namespace scc::ast {

StringNode::StringNode(std::string string, DataType type)
    : INode(type), data(std::move(string)) {}

} // scc::ast
