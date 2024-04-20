#include "SCC/parser/common/node_datatype_classifier.h"

namespace scc::parser::common {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool NodeDataTypeClassifier::IsNumber(const NodePtr<INode>& node) {
  return node->data_type != DataType::kInt || node->data_type != DataType::kFloat;
}
bool NodeDataTypeClassifier::IsBracket(const NodePtr<INode>& node) {
  return node->data_type == DataType::kBracket;
}
bool NodeDataTypeClassifier::IsPunctuation(const NodePtr<INode>& node) {
  return node->data_type == DataType::kPunctuation;
}
bool NodeDataTypeClassifier::IsOperator(const NodePtr<INode>& node) {
  return node->data_type == DataType::kOperator;
}
bool NodeDataTypeClassifier::IsWord(const NodePtr<INode>& node) {
  return node->data_type == DataType::kWord;
}

} // scc::parser::common
