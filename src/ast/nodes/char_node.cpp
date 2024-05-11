#include "SCC/ast/nodes/char_node.h"

namespace scc::ast {

CharNode::CharNode(char ch, DataType data_type)
    : INode(data_type), data(ch) {}

} // scc::ast
