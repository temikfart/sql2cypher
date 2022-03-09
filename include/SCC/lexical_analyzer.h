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
  Node(const N_type type);
  ~Node();

  N_type get_type();

  void add_children(Node* node);
  Node* get_children (int node_num);
  size_t get_children_amount ();

  virtual void print_data() = 0;
};

class IntNode: public Node {
  int data_;
public:
  IntNode(int value): Node (N_type::DIGIT), data_(value) {}

  int get_data() const {
    return data_;
  }

  void print_data() override {
    std::cout << data_ << " ";
  }
};

class CharNode: public Node {
  char data_;
public:
  CharNode(char ch): Node (N_type::OPERATOR), data_(ch) {}

  char get_data() {
    return data_;
  }

  void print_data() override {
    std::cout << data_ << " ";
  }
};

class StringNode: public Node {
  std::string data_;
public:
  StringNode(std::string&& string): Node (N_type::STRING), data_(string) {}

  std::string get_data() {
    return data_;
  }

  void print_data() override {
    std::cout << data_ << " ";
  }
};

class RootNode: public Node {
public:
  RootNode(): Node (N_type::ROOT) {}

  void print_data() override {
    std::cout << "ROOT" << " ";
  }
};

namespace Tree {
  void PrintTreeRecursive(Node* node);
};

void test_tree_func();
