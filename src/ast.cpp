#include "SCC/ast.h"

//-------------------Node------------------------

Node::Node(DataType type) : type_(type) {}

void Node::set_st_type(StatementType type) {
  this->ValidateStType(type);
  st_type_ = type;
}
DataType Node::get_type() {
  return type_;
}
StatementType Node::get_st_type() {
  return st_type_;
}
std::shared_ptr<Node> Node::get_child (size_t node_num) {
  this->ValidateChildNumber(node_num);
  return children_[node_num];
}
size_t Node::get_children_amount () {
  return children_.size();
}
void Node::AddChild(std::shared_ptr<Node> const& node) {
  this->ValidateAddChild(node);
  children_.push_back(node);
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
void Node::ValidateStType(StatementType type) {
  if (StatementType::StTypeCount <= type) {
    LOG(ERROR, "incorrect StatementType: " << type);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "StatementType is valid");
}

//-------------------IntNumNode---------------------
IntNumNode::IntNumNode(int value, DataType type)
  : Node (type), data_(value) {
  this->ValidateType();
}


int IntNumNode::get_data() const {
  return data_;
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

StringNode::StringNode(std::string&& string, DataType type)
  : Node (type), data_(string) {
  this->ValidateType();
}

std::string StringNode::get_data() const{
  return data_;
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