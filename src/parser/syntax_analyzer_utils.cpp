#include "SCC/parser/syntax_analyzer.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> SyntaxAnalyzer::CreateRootNode(StmtType stmt_type) const {
  return CastToINode(std::make_shared<RootNode>(stmt_type));
}
NodePtr<INode> SyntaxAnalyzer::CreateServiceNode(StmtType stmt_type) const {
  return CastToINode(std::make_shared<ServiceNode>(stmt_type));
}
NodePtr<INode> SyntaxAnalyzer::CreateCharNode(char ch, DataType data_type) const {
  return CastToINode(std::make_shared<CharNode>(ch, data_type));
}
NodePtr<INode> SyntaxAnalyzer::CreateStringNode(const std::string& str, DataType data_type) const {
  return CastToINode(std::make_shared<StringNode>(str, data_type));
}

} // scc::parser
