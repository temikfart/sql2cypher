#include "SCC/parser/common/node_datatype_classifier.h"

namespace scc::parser::common {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool NodeDataTypeClassifier::IsBracket(NodePtr<INode>& node) {
  return (node->data_type == DataType::kBracket);
}
bool NodeDataTypeClassifier::IsPunctuation(NodePtr<INode>& node) {
  return (node->data_type == DataType::kPunctuation);
}
bool NodeDataTypeClassifier::IsWord(NodePtr<INode>& node) {
  return (node->data_type == DataType::kWord);
}
bool NodeDataTypeClassifier::IsNumber(NodePtr<INode>& node) {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool NodeDataTypeClassifier::IsOperator(NodePtr<INode>& node) {
  return (node->data_type == DataType::kOperator);
}

} // scc::parser::common
