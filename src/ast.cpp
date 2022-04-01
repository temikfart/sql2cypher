#include "SCC/ast.h"

#include <utility>

//-------------------Node------------------------

Node::Node(DataType type) : type_(type) {}
Node::~Node() {}

DataType Node::get_type() const {
  return type_;
}
std::shared_ptr<Node>& Node::get_child (size_t node_num) {
  this->ValidateChildNumber(node_num);
  return children_[node_num];
}
size_t Node::get_children_amount () const {
  return children_.size();
}

void Node::AddChild(std::shared_ptr<Node> const& node) {
  this->ValidateAddChild(node);
  children_.push_back(node);
}
bool Node::IsNodesEqual(const std::shared_ptr<Node>& node1,
                         const std::shared_ptr<Node>& node2) {
  if (node1->get_type() != node2->get_type() ||
      node1->get_children_amount() != node2->get_children_amount()) { //TODO: add parent ptr comparison
    return false;
  }

  switch (node1->get_type()) {
    case DataType::ROOT:
      break;
    case DataType::INT_NUMBER:
    {
      int data1 = std::dynamic_pointer_cast<IntNumNode>(node1)->get_data();
      int data2 = std::dynamic_pointer_cast<IntNumNode>(node2)->get_data();
    }
      if (std::dynamic_pointer_cast<IntNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<IntNumNode>(node2)->get_data()) {
        return false;
      }
      break;
    case DataType::FLOAT_NUMBER:
      if (std::dynamic_pointer_cast<FloatNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<FloatNumNode>(node2)->get_data()) {
        return false;
      }
      break;
    case DataType::PUNCTUATION: case DataType::BRACKET: //TODO: decide how to check node type: make a functions or make a new field node_type_
      if (std::dynamic_pointer_cast<CharNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<CharNode>(node2)->get_data()) {
        return false;
      }
      break;
    case DataType::WORD: case DataType::OPERATOR:
      if (std::dynamic_pointer_cast<StringNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<StringNode>(node2)->get_data()) {
        return false;
      }
      break;
  }

  size_t children_amount = node1->get_children_amount();
  for (size_t i = 0; i < children_amount; i++) {
    if (!IsNodesEqual(node1->get_child(i),
                      node2->get_child(i)))
      return false;
  }

  return true;
}

void Node::ValidateChildNumber(size_t node_num) const {
  if (node_num >= children_.size()) {
    LOG(ERROR, "node number too big");
    exit(EXIT_FAILURE);
  }
}
void Node::ValidateAddChild(std::shared_ptr<Node> const& node) const {
  if (node == nullptr) {
    LOG(ERROR, "nullptr can not be added to the tree");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "adding node is valid");
}

//-------------------IntNumNode---------------------
IntNumNode::IntNumNode(int value, DataType type)
  : Node (type), data_(value) {
  this->ValidateType();
}

int IntNumNode::get_data() const {
  return data_;
}
void IntNumNode::set_data(const int &data, DataType type) {
  data_ = data;
  type_ = type;
  ValidateType();
}
void IntNumNode::set_data(const int &data) {
  data_ = data;
}


void IntNumNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void IntNumNode::PrintType(std::ostream &stream) {
  stream << "INT_NUMBER";
}

void IntNumNode::ValidateType() const {
  if (this->type_ != DataType::INT_NUMBER) {
    LOG(ERROR, "wrong type for IntNumNode");
    exit(EXIT_FAILURE);
  }
}

//-------------------FloatNumNode---------------------
FloatNumNode::FloatNumNode(double value, DataType type)
  : Node (type), data_(value) {
  this->ValidateType();
}

double FloatNumNode::get_data() const {
  return data_;
}
void FloatNumNode::set_data(const double &data, DataType type) {
  data_ = data;
  type_ = type;
  ValidateType();
}
void FloatNumNode::set_data(const double &data) {
  data_ = data;
}

void FloatNumNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void FloatNumNode::PrintType(std::ostream &stream) {
  stream << "FLOAT_NUMBER";
}

void FloatNumNode::ValidateType() const {
  if (this->type_ != DataType::FLOAT_NUMBER) {
    LOG(ERROR, "wrong type for FloatNumNode");
    exit(EXIT_FAILURE);
  }
}

//-------------------CharNode--------------------

CharNode::CharNode(char ch, DataType type)
  : Node (type), data_(ch) {
  this->ValidateType();
}

char CharNode::get_data() const {
  return data_;
}
void CharNode::set_data(const char &data, DataType type) {
  data_ = data;
  type_ = type;
  ValidateType();
}
void CharNode::set_data(const char &data) {
  data_ = data;

}

void CharNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void CharNode::PrintType(std::ostream &stream) {
  switch (type_) {
    case DataType::BRACKET:
      stream << "BRACKET";
      break;
    case DataType::PUNCTUATION:
      stream << "PUNCTUATION";
      break;
    default:
      LOG(ERROR, "wrong type for CharNode, "
                 "something wrong with validation of type");
      exit(EXIT_FAILURE);
  }
}

void CharNode::ValidateType() const {
  if (this->type_ != DataType::BRACKET
      && this->type_ != DataType::PUNCTUATION) {
    LOG(ERROR, "wrong type for CharNode");
    exit(EXIT_FAILURE);
  }
}

//-------------------StringNode------------------

StringNode::StringNode(std::string string, DataType type)
  : Node (type), data_(std::move(string)) {
  this->ValidateType();
}

std::string StringNode::get_data() const{
  return data_;
}
void StringNode::set_data(const std::string &data, DataType type) {
  data_ = data;
  type_ = type;
  ValidateType();
}
void StringNode::set_data(const std::string &data) {
  data_ = data;
}

void StringNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void StringNode::PrintType(std::ostream &stream) {
  switch (type_) {
    case DataType::WORD:
      stream << "WORD";
      break;
    case DataType::OPERATOR:
      stream << "OPERATOR";
      break;
    default:
      LOG(ERROR, "wrong type for StringNode, "
                 "something wrong with validation of type");
      exit(EXIT_FAILURE);
  }
}

void StringNode::ValidateType() const {
  if (this->type_ != DataType::WORD && this->type_ != DataType::OPERATOR) {
    LOG(ERROR, "wrong type for StringNode");
    exit(EXIT_FAILURE);
  }
}

//-------------------RootNode--------------------

RootNode::RootNode()  : Node(DataType::ROOT) {
  this->ValidateType();
}

void RootNode::PrintData(std::ostream &stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream &stream) {
  stream << "ROOT";
}

void RootNode::ValidateType() const {
  if (this->type_ != DataType::ROOT) {
    LOG(ERROR, "wrong type for RootNode");
    exit(EXIT_FAILURE);
  }
}

//-------------------Tree------------------------

void Tree::PrintTreeRecursive(std::shared_ptr<Node> const &node,
                              std::ostream &stream) {
  node->PrintData(stream);
  stream << std::endl;

  if (node->get_children_amount() == 0)
    return;

  for (size_t i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_child(i), std::cout);
  }
}

//bool operator== (std::shared_ptr<Node> const& lhs,
//                 std::shared_ptr<Node> const& rhs) {
//  if (lhs->get_type() != rhs->get_type() ||
//      lhs->get_children_amount() != rhs->get_children_amount()) {
//    return false;
//  }
//
//  switch (lhs->get_type()) {
//    case DataType::ROOT:
//      return true;
//    case DataType::INT_NUMBER:
//      if (std::dynamic_pointer_cast<IntNumNode>(lhs)->get_data() !=
//          std::dynamic_pointer_cast<IntNumNode>(rhs)->get_data()) {
//        return false;
//      }
//      break;
//    case DataType::FLOAT_NUMBER:
//      if (std::dynamic_pointer_cast<FloatNumNode>(lhs)->get_data() !=
//          std::dynamic_pointer_cast<FloatNumNode>(rhs)->get_data()) {
//        return false;
//      }
//      break;
//    case DataType::PUNCTUATION: case DataType::BRACKET:
//      if (std::dynamic_pointer_cast<CharNode>(lhs)->get_data() !=
//          std::dynamic_pointer_cast<CharNode>(rhs)->get_data()) {
//        return false;
//      }
//      break;
//    case DataType::WORD: case DataType::OPERATOR:
//      if (std::dynamic_pointer_cast<StringNode>(lhs)->get_data() !=
//          std::dynamic_pointer_cast<StringNode>(rhs)->get_data()) {
//        return false;
//      }
//      break;
//  }
//
//  size_t children_amount = lhs->get_children_amount();
//  for (size_t i = 0; i < children_amount; i++) {
//    if (!(lhs->get_child(i) == rhs->get_child(i)))
//      return false;
//  }
//
//  return true;
//}
//
//bool operator!= (std::shared_ptr<Node> const& lhs,
//                 std::shared_ptr<Node> const& rhs) {
//  return !(lhs == rhs);
//}