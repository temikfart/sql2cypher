#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace ast::common;

using std::format;
using cypher::PropertyType;

template<typename ASTNodeType,
    typename std::enable_if<std::is_base_of<INode, ASTNodeType>::value>::type* = nullptr>
using ASTNodePtr = std::shared_ptr<ASTNodeType>;
template<typename T, typename U>
using Pair = std::pair<T, U>;

Pair<PropertyType, std::string>
Translator::TranslateExpression(const ASTNodePtr<INode>& expr) {
  auto value_node = expr->Child(0);

  if (IsCorrectStmtType(value_node, StmtType::kName)) {
    // TODO: property can not be got from node with StmtType::kName stmt type, but expression can.
  }

  if (HasChildren(value_node, 2)) {
    // TODO: property can not be '!expr', but expression can.
  }

  if (IsCorrectStmtType(value_node, StmtType::kExpression)) {
    return TranslateExpression(value_node);
  }

  if (value_node->data_type == DataType::kString) {
    auto value = ASTUtils::CastToNodeType<StringNode>(value_node)->data;
    return {PropertyType::kString, value};
  }

  if (IsCorrectStmtType(value_node, StmtType::kMathExpression)) {
    throw translation_error("Math expressions are not supported yet");
  }

  switch (value_node->data_type) {
    case DataType::kInt: {
      auto value = ASTUtils::CastToNodeType<IntNumNode>(value_node)->data;
      return {PropertyType::kInteger, std::to_string(value)};
    }
    case DataType::kFloat: {
      auto value = ASTUtils::CastToNodeType<FloatNumNode>(value_node)->data;
      return {PropertyType::kFloat, std::to_string(value)};
    }
    case DataType::kWord: {
      auto value = scc::common::LowerCase(ASTUtils::CastToNodeType<StringNode>(value_node)->data);
      if (value == "true" || value == "false") {
        return {PropertyType::kBoolean, value};
      }
    }
    default:
      throw translation_error("Can not translate expression: unsupported value");
  }
}

} // scc::translator
