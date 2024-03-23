#include "SCC/ast/nodes/string_node.h"

StringNode::StringNode(std::string string, DataType type)
    : INode(type), data(std::move(string)) {}

void StringNode::PrintData(std::ostream& stream) {
  stream << data;
}
void StringNode::PrintType(std::ostream& stream) {
  switch (data_type) {
    case DataType::kWord:
      stream << "WORD";
      break;
    case DataType::kOperator:
      stream << "OPERATOR";
      break;
    default:
      LOGE << "wrong type for StringNode, "
              "something wrong with validation of type";
      end(EXIT_FAILURE);
  }
}
