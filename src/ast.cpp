#include "SCC/ast.h"

//-------------------Node------------------------

Node::Node(DataType type): type_(type) {}
Node::~Node() {
    LOG(DEBUG, "Node destructor is called");
}

DataType Node::get_type() {
  return type_;
}
Node* Node::get_child (size_t node_num) {
  this->ValidateGetChild(node_num);
  return children_[node_num];
}
size_t Node::get_children_amount () {
  return children_.size();
}
void Node::AddChild(Node* node) {
  this->ValidateAddChild(node);
  children_.push_back(node);
}

void Node::ValidateGetChild(size_t node_num) {
  if (node_num > children_.size()) {
    LOG(ERROR, "Node number too big");
    exit(EXIT_FAILURE);
  }
}
void Node::ValidateAddChild(Node* node){
  if (node == nullptr) {
    LOG(ERROR, "Node is nullptr");
    exit(EXIT_FAILURE);
  }
}

//-------------------IntNumNode---------------------
IntNumNode::IntNumNode(int value, DataType type): Node (type), data_(value) {}
IntNumNode::~IntNumNode() {
    for(auto & node_ptr : children_) {
        delete node_ptr;
    }
}

int IntNumNode::get_data() const {
  return data_;
}

void IntNumNode::PrintData() {
  std::cout << data_ << " ";
}
void IntNumNode::PrintType() {
  std::cout << "INT_NUMBER ";
}

//-------------------FloatNumNode---------------------
FloatNumNode::FloatNumNode(double value, DataType type): Node (type), data_(value) {}
FloatNumNode::~FloatNumNode() {
  for(auto & node_ptr : children_) {
    delete node_ptr;
  }
}

double FloatNumNode::get_data() const {
  return data_;
}

void FloatNumNode::PrintData() {
  std::cout << data_ << " ";
}
void FloatNumNode::PrintType() {
  std::cout << "FLOAT_NUMBER ";
}

//-------------------CharNode--------------------

CharNode::CharNode(char ch, DataType type): Node (type), data_(ch) {}
CharNode::~CharNode() {
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

char CharNode::get_data() const {
  return data_;
}

void CharNode::PrintData() {
  std::cout << data_ << " ";
}
void CharNode::PrintType() {
  std::cout << "CHAR ";
}

//-------------------StringNode------------------

StringNode::StringNode(std::string&& string, DataType type): Node (type), data_(string) {}
StringNode::~StringNode() {
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

std::string StringNode::get_data() const{
  return data_;
}

void StringNode::PrintData(){
  std::cout << data_ << " ";
}
void StringNode::PrintType() {
  std::cout << "WORD ";
}

//-------------------RootNode--------------------

RootNode::RootNode(DataType type): Node (type) {}
RootNode::~RootNode() {
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

void RootNode::PrintData() {
  std::cout << "ROOT" << " ";
}
void RootNode::PrintType() {
  std::cout << "ROOT ";
}

//-------------------Tree------------------------

void Tree::PrintTreeRecursive(Node *node) {
  node->PrintData();
  std::cout << std::endl;

  if (node->get_children_amount() == 0)
    return;

  for (size_t i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_child(i));
  }
}
void Tree::TestTree() {
  Node* root = new RootNode();

  root->AddChild(new IntNumNode(1));
  root->AddChild(new CharNode('?',  DataType::OPERATOR));
  root->AddChild(new StringNode("string", DataType::WORD));
  root->AddChild(new IntNumNode(1));

  Node* child_node = root->get_child(0);
  std::cout << std::endl << static_cast<IntNumNode*>(child_node)->get_data() << std::endl;

  Tree::PrintTreeRecursive(root);

  delete root;
}