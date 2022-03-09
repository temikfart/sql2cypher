#pragma once

#include "SCC/conf.h"
#include <iostream>
#include <vector>

enum N_type {
  ROOT,
  DIGIT,
  OPERATOR,
  STRING
};

class Node {
  const N_type type_;
  std::vector<Node*> children_;

public:
  Node(const N_type type): type_(type) {}
  ~Node() {
    for(auto & node_ptr : children_) {
      delete node_ptr;
    }
  }

  N_type get_type () {
    return type_;
  }

  void add_children(Node* node) {
    children_.push_back(node);
  }
  Node* get_children (int node_num) {
    if (node_num > children_.size()) {
      LOG(ERROR, "Node number too big")
      return nullptr;
    }
    return children_[node_num];
  }
  size_t get_children_amount () {
    return children_.size();
  }
};

class IntNode: public Node {
  int data_;
public:
  IntNode(int value): Node (N_type::DIGIT), data_(value) {}

  int get_data() const {
    return data_;
  }
};

class CharNode: public Node {
  char data_;
public:
  CharNode(char ch): Node (N_type::OPERATOR), data_(ch) {}

  char get_data() {
    return data_;
  }
};

class StringNode: public Node {
  std::string data_;
public:
  StringNode(std::string&& string): Node (N_type::STRING), data_(string) {}

  std::string get_data() {
    return data_;
  }
};

namespace Tree {
  void PrintTreeRecursive(Node* node);
};

void test_tree_func();
