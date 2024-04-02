#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool Parser::IsBracket(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kBracket);
}
bool Parser::IsPunctuation(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kPunctuation);
}
bool Parser::IsWord(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kWord);
}
bool Parser::IsNumber(NodePtr<INode>& node) const {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool Parser::IsOperator(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kOperator);
}

} // scc::parser
