#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "SCC/config.h"

enum StatementType {
  EMPTY_TYPE,
  Program,
  query,
  ddlStatement,
  dmlStatement,
  alterTableStatement,
  createDatabaseStatement,
  createTableStatement,
  dropDatabaseStatement,
  dropTableStatement,
  deleteStatement,
  insertStatement,
  updateStatement,
  name,
  identifier,
  delimiter_dot,
  StTypeCount
};

enum DataType {
  ROOT,
  INT_NUMBER,
  FLOAT_NUMBER,
  BRACKET,
  // Brackets
  PUNCTUATION,
  // Punctuation
  DOT,
  COMMA,
  //
  OPERATOR,
  WORD
};

class Node {
protected:
  const DataType type_;
  StatementType st_type_ = StatementType::EMPTY_TYPE;
  std::shared_ptr<Node> parent_ = nullptr;
  std::vector<std::shared_ptr<Node>> children_;

public:
  explicit Node(DataType type);

  void set_st_type(StatementType type);
  void set_parent(std::shared_ptr<Node>& parent);
  DataType get_type();
  StatementType get_st_type();
  std::shared_ptr<Node>& get_child (size_t node_num);
  size_t get_children_amount ();
  std::shared_ptr<Node>& get_parent();
  void AddChild(std::shared_ptr<Node> const& node);

  virtual void PrintData(std::ostream &stream) = 0;
  virtual void PrintType(std::ostream &stream) = 0;

private:
  void ValidateChildNumber(size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<Node> const& node) const;
  void ValidateStType(StatementType type);
  virtual void ValidateType() const = 0;
};

class IntNumNode: public Node {
  int data_;
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  int get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType() const override;
};

class FloatNumNode: public Node {
  double data_;
public:
  explicit FloatNumNode(double value, DataType type = DataType::FLOAT_NUMBER);

  double get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType() const override;
};

class CharNode: public Node {
  char data_;
public:
  explicit CharNode(char ch, DataType type);

  char get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType() const override;
};

class StringNode: public Node {
  std::string data_;
public:
  explicit StringNode(std::string&& string, DataType type);

  std::string get_data() const;

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType() const override;
};

class RootNode: public Node {
public:
  explicit RootNode();

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType() const override;
};

namespace Tree {
  void PrintTreeRecursive(std::shared_ptr<Node> const &node,
                          std::ostream &stream);
};
