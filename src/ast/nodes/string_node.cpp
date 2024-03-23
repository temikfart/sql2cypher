#include "SCC/ast/nodes/string_node.h"

StringNode::StringNode(std::string string, DataType type)
    : INode(type), data_(std::move(string)) {
  this->ValidateType(type);
}

void StringNode::set_data(const std::string& data) {
  data_ = data;
}
std::string StringNode::get_data() const {
  return data_;
}

void StringNode::PrintData(std::ostream& stream) {
  stream << data_;
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

void StringNode::ValidateType(DataType type) const {
  bool is_WORD = type == DataType::kWord;
  bool is_OPERATOR = type == DataType::kOperator;
  bool is_STRING = type == DataType::kString;
  if (!(is_WORD || is_OPERATOR || is_STRING)) {
    LOGE << "invalid type for StringNode: " << type;
    end(EXIT_FAILURE);
  }
}
