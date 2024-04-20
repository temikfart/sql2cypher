#include "SCC/parser/common/utils.h"

namespace scc::parser::common {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> ParserUtils::CreateRootNode(StmtType stmt_type) {
  return CastToINode(std::make_shared<RootNode>(stmt_type));
}
NodePtr<INode> ParserUtils::CreateServiceNode(StmtType stmt_type) {
  return CastToINode(std::make_shared<ServiceNode>(stmt_type));
}
NodePtr<INode> ParserUtils::CreateCharNode(char ch, DataType data_type) {
  return CastToINode(std::make_shared<CharNode>(ch, data_type));
}
NodePtr<INode> ParserUtils::CreateStringNode(const std::string& str, DataType data_type) {
  return CastToINode(std::make_shared<StringNode>(str, data_type));
}

} // scc::parser::common
