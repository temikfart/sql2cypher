#include "SCC/ast/nodes/inode.h"

#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/string_node.h"

INode::INode(DataType data_type) : data_type(data_type) {}
INode::~INode() = default;

void INode::set_parent(std::shared_ptr<INode>& node) {
  parent_ = node;
}
std::shared_ptr<INode>& INode::get_child(size_t node_num) {
  this->ValidateChildNumber(node_num);
  return children_[node_num];
}
size_t INode::get_children_amount() const {
  return children_.size();
}

void INode::AddChild(std::shared_ptr<INode> const& node) {
  this->ValidateAddChild(node);
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
    case DataType::ROOT:
      break;
    case DataType::INT_NUMBER:
      if (std::dynamic_pointer_cast<IntNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<IntNumNode>(node2)->get_data()) {
        LOGT << "not equal: different integer number data";
        return false;
      }
      break;
    case DataType::FLOAT_NUMBER:
      if (std::dynamic_pointer_cast<FloatNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<FloatNumNode>(node2)->get_data()) {
        LOGT << "not equal: different float number data";
        return false;
      }
      break;
    case DataType::PUNCTUATION:
    case DataType::BRACKET:
      if (std::dynamic_pointer_cast<CharNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<CharNode>(node2)->get_data()) {
        return false;
      }
      break;
    case DataType::WORD:
    case DataType::OPERATOR:
      if (std::dynamic_pointer_cast<StringNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<StringNode>(node2)->get_data()) {
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

void INode::ValidateChildNumber(size_t node_num) const {
  if (node_num >= children_.size()) {
    LOGE << "node number too big";
    end(EXIT_FAILURE);
  }
}
void INode::ValidateAddChild(std::shared_ptr<INode> const& node) const {
  if (node == nullptr) {
    LOGE << "nullptr can not be added to the tree";
    end(EXIT_FAILURE);
  }
}

void Tree::PrintTreeRecursive(std::shared_ptr<INode> const& node,
                              std::ostream& stream) {
  node->PrintData(stream);
  stream << std::endl;

  if (node->get_children_amount() == 0)
    return;

  for (size_t i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_child(i), std::cout);
  }
}
