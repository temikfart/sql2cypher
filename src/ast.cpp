#include "SCC/ast.h"

//-------------------Node------------------------

Node::Node(NodeType type): type_(type) {}
Node::~Node() {
    LOG(DEBUG, "Node destructor is called");
}

NodeType Node::get_type() {
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
    LOG(ERROR, "node is nullptr");
    exit(EXIT_FAILURE);
  }
}

//-------------------NumNode---------------------
template <typename Data_T>
NumNode<Data_T>::NumNode(Data_T value): Node (NodeType::NUMBER), data_(value) {}
template <typename Data_T>
NumNode<Data_T>::~NumNode() {
    LOG(DEBUG, "NumNode destructor is called");
    for(auto & node_ptr : children_) {
        delete node_ptr;
    }
}

template <typename Data_T>
Data_T NumNode<Data_T>::get_data() const {
  return data_;
}

template <typename Data_T>
void NumNode<Data_T>::PrintData() {
  std::cout << data_ << " ";
}

//-------------------CharNode--------------------

CharNode::CharNode(char ch): Node (NodeType::OPERATOR), data_(ch) {}
CharNode::~CharNode() {
    LOG(DEBUG, "CharNode destructor is called");
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

//-------------------StringNode------------------

StringNode::StringNode(std::string&& string): Node (NodeType::WORD), data_(string) {}
StringNode::~StringNode() {
    LOG(DEBUG, "StringNode destructor is called");
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

//-------------------RootNode--------------------

RootNode::RootNode(): Node (NodeType::ROOT) {}
RootNode::~RootNode() {
    LOG(DEBUG, "RootNode destructor is called");
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

void RootNode::PrintData() {
  std::cout << "ROOT" << " ";
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

  root->AddChild(new NumNode<int>(1));
  root->AddChild(new CharNode('?'));
  root->AddChild(new StringNode("string"));

  Node* child_node = root->get_child(0);
  std::cout << std::endl << static_cast<NumNode<int>*>(child_node)->get_data() << std::endl;

  Tree::PrintTreeRecursive(root);

  delete root;
}