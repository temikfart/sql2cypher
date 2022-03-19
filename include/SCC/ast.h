#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "SCC/config.h"

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
  std::vector<std::shared_ptr<Node>> children_;

public:
  explicit Node(DataType type);

  DataType get_type();
  std::shared_ptr<Node> get_child (size_t node_num);
  size_t get_children_amount ();
  void AddChild(std::shared_ptr<Node> const& node);

  virtual void PrintData(std::ostream &stream) = 0;
  virtual void PrintType(std::ostream &stream) = 0;

private:
  void ValidateChildNumber(size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<Node> const& node) const;
};

class IntNumNode: public Node {
  int data_;
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  int get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;
};

class FloatNumNode: public Node {
  double data_;
public:
  explicit FloatNumNode(double value, DataType type = DataType::FLOAT_NUMBER);

  double get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

};

class CharNode: public Node {
  char data_;
public:
  explicit CharNode(char ch, DataType type);

  char get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

};

class StringNode: public Node {
  std::string data_;
public:
  explicit StringNode(std::string&& string, DataType type);

  std::string get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

};

class RootNode: public Node {
public:
  explicit RootNode(DataType type = DataType::ROOT);

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;
};

namespace Tree {
  void PrintTreeRecursive(std::shared_ptr<Node> const &node,
                          std::ostream &stream);
};
