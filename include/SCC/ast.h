#pragma once

#include "SCC/config.h"
#include <iostream>
#include <vector>

enum NodeType {
  ROOT,
  NUMBER,
  OPERATOR,
  WORD
};

class Node {
protected:
  const NodeType type_;
  std::vector<Node*> children_;

public:
  explicit Node(NodeType type);
  virtual ~Node();

  NodeType get_type();
  Node* get_child (size_t node_num);
  size_t get_children_amount ();
  void AddChild(Node* node);

//  virtual char get_data() = 0;
//  virtual std::string get_data() = 0;  TODO: How to make get_data virtual method?

  void ValidateGetChild(size_t node_num);
  void ValidateAddChild(Node* node);

  virtual void PrintData() = 0;
};

template <typename Data_T>
class NumNode: public Node {
  Data_T data_;
public:
  explicit NumNode(Data_T value);
  ~NumNode() override;

  Data_T get_data() const;

  void PrintData() override;
};

class CharNode: public Node {
  char data_;
public:
  explicit CharNode(char ch);
  ~CharNode() override;

  char get_data() const;

  void PrintData() override;
};

class StringNode: public Node {
  std::string data_;
public:
  explicit StringNode(std::string&& string);
  ~StringNode() override;

  std::string get_data() const;

  void PrintData() override;
};

class RootNode: public Node {
public:
  RootNode();
  ~RootNode() override;

  void PrintData() override;
};

namespace Tree {
  void PrintTreeRecursive(Node* node);

  void TestTree();
};
