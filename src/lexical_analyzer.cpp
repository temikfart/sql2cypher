#include "SCC/lexical_analyzer.h"

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

Node* Node::get_children (int node_num) {
  if (node_num > children_.size()) {
    LOG(ERROR, "Node number too big")
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

//-------------------IntNode---------------------

IntNode::IntNode(int value): Node (N_type::NUMBER), data_(value) {}

int IntNode::get_data() const {
  return data_;
}

void IntNode::PrintData() {
  std::cout << data_ << " ";
}

//-------------------CharNode--------------------

CharNode::CharNode(char ch): Node (N_type::OPERATOR), data_(ch) {}

char CharNode::get_data() {
  return data_;
}

void CharNode::PrintData() {
  std::cout << data_ << " ";
}

//-------------------StringNode------------------

StringNode::StringNode(std::string&& string): Node (N_type::WORD), data_(string) {}

std::string StringNode::get_data() {
  return data_;
}

void StringNode::PrintData(){
  std::cout << data_ << " ";
}

//-------------------RootNode--------------------

RootNode::RootNode(): Node (N_type::ROOT) {}

void RootNode::PrintData() {
  std::cout << "ROOT" << " ";
}

//-------------------Tree------------------------

void Tree::PrintTreeRecursive(Node *node) {
  node->PrintData();

  if (node->get_children_amount() == 0)
    return;

  for (int i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_children(i));
  }
}

void Tree::TestTree() {
  Node* root = new RootNode();

  root->add_children(new IntNode(125));
  root->add_children(new CharNode('+'));
  root->add_children(new StringNode("le cringe"));
  root->add_children(new IntNode(125));

  Node* child_node = root->get_children(0);

  child_node->add_children(new StringNode("lol"));

  Tree::PrintTreeRecursive(root);

  delete root;
}