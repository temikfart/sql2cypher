#include "SCC/ast/nodes/char_node.h"

CharNode::CharNode(char ch, DataType data_type)
    : INode(data_type), data(ch) {}

void CharNode::PrintData(std::ostream& stream) {
  stream << data;
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
