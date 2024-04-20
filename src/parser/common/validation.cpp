#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Parser::ValidateNotEmpty() const {
  if (tokens_.empty()) {
    LOGE << "expected that tokens' array is not empty";
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsWord(NodePtr<INode>& node) const {
  if (node->data_type != DataType::kWord) {
    LOGE << "expected word in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsOpeningRoundBracket(NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsClosingRoundBracket(NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsSingleQuote(NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsDoubleQuote(NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}

} // scc::parser
