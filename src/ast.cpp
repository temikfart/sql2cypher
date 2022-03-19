#include "SCC/ast.h"

//-------------------Node------------------------

Node::Node(DataType type): type_(type) {}

DataType Node::get_type() {
  return type_;
}
std::shared_ptr<Node> Node::get_child (size_t node_num) {
  this->ValidateChildNumber(node_num);
  return children_[node_num];
}
size_t Node::get_children_amount () {
  return children_.size();
}
void Node::AddChild(std::shared_ptr<Node> const& node) {
//  this->ValidateAddChild(node); //TODO: how to validate shared_ptr
  children_.push_back(node);
}

void Node::ValidateChildNumber(size_t node_num) {
  if (node_num >= children_.size()) {
    LOG(ERROR, "Node number too big");
    exit(EXIT_FAILURE);
  }
}

//-------------------IntNumNode---------------------
IntNumNode::IntNumNode(int value, DataType type): Node (type), data_(value) {}


int IntNumNode::get_data() const {
  return data_;
}

void IntNumNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void IntNumNode::PrintType(std::ostream &stream) {
  stream << "INT_NUMBER";
}

//-------------------FloatNumNode---------------------
FloatNumNode::FloatNumNode(double value, DataType type): Node (type),
                                                         data_(value) {}

double FloatNumNode::get_data() const {
  return data_;
}

void FloatNumNode::PrintData(std::ostream &stream) {
  stream << data_;
}
void FloatNumNode::PrintType(std::ostream &stream) {
  stream << "FLOAT_NUMBER ";
}

//-------------------CharNode--------------------

CharNode::CharNode(char ch, DataType type): Node (type), data_(ch) {}

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
      stream << "WRONG_TYPE_FOR_THIS_NODE";
  }
}

//-------------------StringNode------------------

StringNode::StringNode(std::string&& string, DataType type): Node (type),
                                                             data_(string) {}

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
      stream << "WRONG_TYPE_FOR_THIS_NODE";
  }
}

//-------------------RootNode--------------------

RootNode::RootNode(DataType type): Node (type) {}

void RootNode::PrintData(std::ostream &stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream &stream) {
  stream << "ROOT";
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
void Tree::TestTree() {

  std::vector<std::shared_ptr<Node>> Nodes;
  Nodes.push_back(std::make_shared<IntNumNode>(5));
  Nodes.push_back(std::make_shared<IntNumNode>(6));
  Nodes.push_back(std::make_shared<StringNode>("Hello",
                                               DataType::WORD));

  std::shared_ptr<IntNumNode> root = std::make_shared<IntNumNode>(5);
  std::shared_ptr<IntNumNode> child1 = std::make_shared<IntNumNode>(10);
  std::shared_ptr<StringNode> child2 = std::make_shared<StringNode>
    ("Hello", DataType::WORD);
  root->AddChild(child1);
  root->AddChild(child2);

  std::cout << root->get_data() << std::endl;
  std::cout << child1->get_data() << std::endl;
  std::cout << child2->get_data() << std::endl;

  root->get_data();
}