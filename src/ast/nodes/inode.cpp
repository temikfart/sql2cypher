#include "SCC/ast/nodes/inode.h"

#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/string_node.h"

namespace scc::ast {

INode::INode(DataType data_type) : data_type(data_type) {}
INode::~INode() = default;

std::shared_ptr<INode> INode::get_child(std::size_t node_num) {
  if (node_num >= children_.size())
    return {};
  return children_[node_num];
}
std::size_t INode::get_children_amount() const {
  return children_.size();
}

void INode::AddChild(std::shared_ptr<INode> const& node) {
  children_.push_back(node);
}
bool INode::IsNodesEqual(const std::shared_ptr<INode>& node1,
                         const std::shared_ptr<INode>& node2) {
  if (node1->data_type != node2->data_type ||
      node1->get_children_amount()
          != node2->get_children_amount()) { //TODO: add parent ptr comparison
    LOGT << "not equal: comparing nodes with different "
               "numbers of children or have different types";
    return false;
  }

  switch (node1->data_type) {
    case DataType::kRoot:
      break;
    case DataType::kInt:
      if (std::dynamic_pointer_cast<IntNumNode>(node1)->data !=
          std::dynamic_pointer_cast<IntNumNode>(node2)->data) {
        LOGT << "not equal: different integer number data";
        return false;
      }
      break;
    case DataType::kFloat:
      if (std::dynamic_pointer_cast<FloatNumNode>(node1)->data !=
          std::dynamic_pointer_cast<FloatNumNode>(node2)->data) {
        LOGT << "not equal: different float number data";
        return false;
      }
      break;
    case DataType::kPunctuation:
    case DataType::kBracket:
      if (std::dynamic_pointer_cast<CharNode>(node1)->data !=
          std::dynamic_pointer_cast<CharNode>(node2)->data) {
        return false;
      }
      break;
    case DataType::kWord:
    case DataType::kOperator:
      if (std::dynamic_pointer_cast<StringNode>(node1)->data !=
          std::dynamic_pointer_cast<StringNode>(node2)->data) {
        LOGT << "not equal: different string data";
        return false;
      }
      break;
    default:
      LOGE << "invalid Datatype: " << node1->data_type;
      end(EXIT_FAILURE);
  }

  size_t children_amount = node1->get_children_amount();
  for (size_t i = 0; i < children_amount; i++) {
    if (!IsNodesEqual(node1->get_child(i),
                      node2->get_child(i))) {
      LOGT << "not equal: different subtrees";
      return false;
    }
  }

  LOGT << "nodes are equal";
  return true;
}

} // scc::ast
