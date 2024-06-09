#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace ast::common;

using std::format;

template<typename ASTNodeType,
    typename std::enable_if<std::is_base_of<INode, ASTNodeType>::value>::type* = nullptr>
using ASTNodePtr = std::shared_ptr<ASTNodeType>;

std::string Translator::TranslateMathExpression(const ASTNodePtr<INode>& expr) const {
  std::stringstream ss;

  auto sum_node = expr->Child(0);
  ValidateIsCorrectStmtType(sum_node, StmtType::kSum);
  ValidateHasChildren(sum_node);
  auto sum = TranslateMathSum(sum_node);
  ss << sum;

  return ss.str();
}
std::string Translator::TranslateMathSum(const ASTNodePtr<INode>& sum) const {
  std::stringstream ss;

  auto left_product_node = sum->Child(0);
  ValidateIsCorrectStmtType(left_product_node, StmtType::kProduct);
  ValidateHasChildren(left_product_node);
  auto left_product = TranslateMathProduct(left_product_node);
  ss << left_product;

  if (HasChildren(sum, 2)) {
    ValidateHasChildren(sum, 3);
    auto operator_str = ASTUtils::CastToNodeType<StringNode>(sum->Child(1))->data;

    auto right_product_node = sum->Child(2);
    ValidateIsCorrectStmtType(right_product_node, StmtType::kProduct);
    ValidateHasChildren(right_product_node);
    auto right_product = TranslateMathProduct(right_product_node);

    ss << format(" {} {}", operator_str, right_product);
  }

  return ss.str();
}
std::string Translator::TranslateMathProduct(const ASTNodePtr<INode>& product) const {
  std::stringstream ss;

  auto left_power_node = product->Child(0);
  ValidateIsCorrectStmtType(left_power_node, StmtType::kPower);
  ValidateHasChildren(left_power_node);
  auto left_power = TranslateMathPower(left_power_node);
  ss << left_power;

  if (HasChildren(product, 2)) {
    ValidateHasChildren(product, 3);
    auto operator_str = ASTUtils::CastToNodeType<StringNode>(product->Child(1))->data;

    auto right_power_node = product->Child(2);
    ValidateIsCorrectStmtType(right_power_node, StmtType::kPower);
    ValidateHasChildren(right_power_node);
    auto right_power = TranslateMathPower(right_power_node);

    ss << format(" {} {}", operator_str, right_power);
  }

  return ss.str();
}
std::string Translator::TranslateMathPower(const ASTNodePtr<INode>& power) const {
  std::stringstream ss;

  auto value_node = power->Child(0);
  ValidateIsCorrectStmtType(value_node, StmtType::kValue);
  ValidateHasChildren(value_node);
  auto value = TranslateMathValue(value_node);
  ss << value;

  if (HasChildren(power, 2)) {
    ValidateHasChildren(power, 3);
    auto operator_str = ASTUtils::CastToNodeType<StringNode>(power->Child(1))->data;

    auto next_power_node = power->Child(2);
    ValidateIsCorrectStmtType(next_power_node, StmtType::kPower);
    ValidateHasChildren(next_power_node);
    auto next_power = TranslateMathPower(next_power_node);

    ss << format("{}{}", operator_str, next_power);
  }

  return ss.str();
}
std::string Translator::TranslateMathValue(const ASTNodePtr<INode>& value) const {
  std::stringstream ss;

  auto node = value->Child(0);
  if (IsCorrectStmtType(node, StmtType::kMathExpression)) {
    ValidateHasChildren(node);
    auto math_expression = TranslateMathExpression(node);
    return format("({})", math_expression);
  } else {
    switch (node->data_type) {
      case DataType::kInt:
        return std::to_string(ASTUtils::CastToNodeType<IntNumNode>(node)->data);
      case DataType::kFloat:
        return std::to_string(ASTUtils::CastToNodeType<FloatNumNode>(node)->data);
      default:
        std::string msg = format("Unknown value data type: {}", node->data_type.ToString());
        throw translation_error(msg);
    }
  }
}

} // scc::translator
