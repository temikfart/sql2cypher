#include "SCC/ast/nodes/char_node.h"

CharNode::CharNode(char ch, DataType type)
    : INode(type), data_(ch) {
  this->ValidateType(type);
}

char CharNode::get_data() const {
  return data_;
}

void CharNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void CharNode::PrintType(std::ostream& stream) {
  switch (data_type) {
    case DataType::kBracket:
      stream << "BRACKET";
      break;
    case DataType::kPunctuation:
      stream << "PUNCTUATION";
      break;
    default:
      LOGE << "wrong type for CharNode, "
              "something wrong with validation of type";
      end(EXIT_FAILURE);
  }
}

void CharNode::ValidateType(DataType type) const {
  bool is_BRACKET = type == DataType::kBracket;
  bool is_PUNCTUATION = type == DataType::kPunctuation;
  if (!(is_BRACKET || is_PUNCTUATION)) {
    LOGE << "invalid type for CharNode: " << type;
    end(EXIT_FAILURE);
  }
}
