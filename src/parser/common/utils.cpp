#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::CreateRootNode(StmtType stmt_type) const {
  return CastToINode(std::make_shared<RootNode>(stmt_type));
}
NodePtr<INode> Parser::CreateServiceNode(StmtType stmt_type) const {
  return CastToINode(std::make_shared<ServiceNode>(stmt_type));
}
NodePtr<INode> Parser::CreateCharNode(char ch, DataType data_type) const {
  return CastToINode(std::make_shared<CharNode>(ch, data_type));
}
NodePtr<INode> Parser::CreateStringNode(const std::string& str, DataType data_type) const {
  return CastToINode(std::make_shared<StringNode>(str, data_type));
}

} // scc::parser
