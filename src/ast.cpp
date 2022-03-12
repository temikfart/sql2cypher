#include "SCC/ast.h"

//-------------------Node------------------------

Node::Node(const N_type type): type_(type) {}

Node::~Node() {
  for(auto & node_ptr : children_) {
    delete node_ptr;
  }
}

N_type Node::get_type() {
  return type_;
}

Node* Node::get_children (size_t node_num) {
  if (node_num > children_.size()) {
    LOG(ERROR, "Node number too big");
    return nullptr;
  }
  return children_[node_num];
}

void Node::add_children(Node* node) {
  children_.push_back(node);
}

size_t Node::get_children_amount () {
  return children_.size();
}

//-------------------NumNode---------------------
template <typename Data_T>
NumNode<Data_T>::NumNode(Data_T value): Node (N_type::NUMBER), data_(value) {}

template <typename Data_T>
NumNode<Data_T>::~NumNode() {
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

CharNode::CharNode(char ch): Node (N_type::OPERATOR), data_(ch) {}

CharNode::~CharNode() {
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

char CharNode::get_data() {
  return data_;
}

void CharNode::PrintData() {
  std::cout << data_ << " ";
}

//-------------------StringNode------------------

StringNode::StringNode(std::string&& string): Node (N_type::WORD), data_(string) {}
StringNode::~StringNode() {
    for (auto &node_ptr: children_) {
        delete node_ptr;
    }
}

std::string StringNode::get_data() {
  return data_;
}

void StringNode::PrintData(){
  std::cout << data_ << " ";
}

//-------------------RootNode--------------------

RootNode::RootNode(): Node (N_type::ROOT) {}
RootNode::~RootNode() {
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

  if (node->get_children_amount() == 0)
    return;

  for (size_t i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_children(i));
  }
}

void Tree::TestTree() {
  Node* root = new RootNode();

  root->add_children(new NumNode<int>(125));
  root->add_children(new CharNode('?'));
  root->add_children(new StringNode("le cringe"));
  root->add_children(new NumNode<double>(125.123));

  Node* child_node = root->get_children(0);

  child_node->add_children(new StringNode("lol"));

  std::cout << std::endl << static_cast<NumNode<int>*>(child_node)->get_data() << std::endl; //TODO: fix this cringe

  Tree::PrintTreeRecursive(root);

  delete root;
}