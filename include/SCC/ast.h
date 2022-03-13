#pragma once

#include "SCC/config.h"
#include <iostream>
#include <vector>

enum DataType {
  ROOT,
  INT_NUMBER,
  FLOAT_NUMBER,
  BRACKET,
  PUNCTUATION,
  OPERATOR,
  WORD
};

class Node {
protected:
  const DataType type_;
  std::vector<Node*> children_;

public:
  explicit Node(DataType type);
  virtual ~Node();

  DataType get_type();
  Node* get_child (size_t node_num);
  size_t get_children_amount ();
  void AddChild(Node* node);

//  virtual char get_data() = 0;
//  virtual std::string get_data() = 0;  TODO: How to make get_data virtual method?

  virtual void PrintData() = 0;
  virtual void PrintType() = 0;

private:
  void ValidateGetChild(size_t node_num);
  void ValidateAddChild(Node* node);
};

class IntNumNode: public Node {
  int data_;
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);
  ~IntNumNode() override;

  int get_data() const;

  void PrintData() override;
  void PrintType() override;
};

class FloatNumNode: public Node {
  double data_;
public:
  explicit FloatNumNode(double value, DataType type = DataType::FLOAT_NUMBER);
  ~FloatNumNode() override;

  double get_data() const;

  void PrintData() override;
  void PrintType() override;

};

class CharNode: public Node {
  char data_;
public:
  explicit CharNode(char ch, DataType type);
  ~CharNode() override;

  char get_data() const;

  void PrintData() override;
  void PrintType() override;

};

class StringNode: public Node {
  std::string data_;
public:
  explicit StringNode(std::string&& string, DataType type);
  ~StringNode() override;

  std::string get_data() const;

  void PrintData() override;
  void PrintType() override;

};

class RootNode: public Node {
public:
  RootNode(DataType type = DataType::ROOT);
  ~RootNode() override;

  void PrintData() override;
  void PrintType() override;
};

namespace Tree {
  void PrintTreeRecursive(Node* node);

  void TestTree();
};
