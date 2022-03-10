#pragma once

#include "SCC/conf.h"
#include <iostream>
#include <vector>

enum N_type {
  ROOT,
  NUMBER,
  OPERATOR,
  WORD
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

  virtual void PrintData() = 0;
};

class IntNode: public Node {
  int data_;
public:
  IntNode(int value);

  int get_data() const;

  void PrintData() override;
};

class CharNode: public Node {
  char data_;
public:
  CharNode(char ch);

  char get_data();

  void PrintData() override;
};

class StringNode: public Node {
  std::string data_;
public:
  StringNode(std::string&& string);

  std::string get_data();

  void PrintData() override;
};

class RootNode: public Node {
public:
  RootNode();

  void PrintData() override;
};

namespace Tree {
  void PrintTreeRecursive(Node* node);

  void TestTree();
};
