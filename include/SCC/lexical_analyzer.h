#pragma once
#include <iostream>
#include <vector>

//union data_t {
//  int digit;
//  char character;
//  std::string string;
//};

enum DataType {
  INT,
  CHAR,
  STRING
};

class Node {
public:
  int type_;
  int data_;
  std::vector<Node*> children_;

  Node(int value = 0): data_(value) {}
  ~Node() {
    for(auto & node_ptr : children_) {
      delete node_ptr;
    }
  }

  Node* get_children(int child_num) {
    if (child_num > children_.size()) {
      std::cout << "get_children: wrong size" << std::endl;
      return nullptr;
    }
    return children_[child_num];
  }

  int get_data() {
    return data_;
  }
  void add_children(int value = 0){
    children_.push_back(new Node);
    children_.back()->data_ = value;
  }

//  virtual void add_children(int value = 0);

  void PrintTree() {
    PrintTreeRecursive(this);
    std::cout << std::endl;
  }
  void PrintTreeRecursive(Node* node) {
    std::cout <<"node_data("<< node->data_ << ") ";
    if (node->children_.empty())
      return;
    std::cout << std::endl;
    for (auto &node_prt: node->children_) {
      PrintTreeRecursive(node_prt);
    }
  }
};

//class IntNode: public Node {
//  int data_;
//
//  int get_data() {
//    return data_;
//  }
//
//  void add_children(int value = 0) override{
//    children_.push_back(new Node);
//  }
//};

//class Tree: public Node {
//public:
//  Node* root_;
//
//  Tree(int value = 0) : Node(value), root_(new Node(value)) {} //why do we need to init Node(value)
//  ~Tree() {
//    delete root_;
//  }
//
//  void PrintTree() {
//    PrintTreeRecursive(root_);
//    std::cout << std::endl;
//  }
//
//  void PrintTreeRecursive(Node* node) {
//    if(node->children_.empty())
//      return;
//    std::cout << node->data_ << " " << std::endl;
//    for(auto & node_prt : node->children_) {
//      PrintTreeRecursive(node_prt);
//    }
//  }
//};

void test_tree_func();