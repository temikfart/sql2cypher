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

  // DDL statements
  alterTableStatement,
  createDatabaseStatement,
  createTableStatement,
  dropDatabaseStatement,
  dropTableStatement,

  // DML statements
  deleteStatement,
  insertStatement,
  updateStatement,

  // Basic statement
  tableDefinition,
  columnDefinition,
  tableConstraint,
  primaryKey,
  foreignKey,
  name,
  identifier,
  delimiter_dot,

  // SQL datatypes
  SQL_int,
  SQL_float,
  SQL_char,
  SQL_varchar,

  StTypeCount
};

enum DataType {
  ROOT,
  INT_NUMBER,
  FLOAT_NUMBER,
  BRACKET,
  // Brackets
  ROUND_BRACKET,
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
  DataType type_;
  StatementType st_type_ = StatementType::EMPTY_TYPE;
  std::shared_ptr<Node> parent_ = nullptr;
  std::vector<std::shared_ptr<Node>> children_;

public:
  explicit Node(DataType type);
  virtual ~Node();

  void set_st_type(StatementType type);
  void set_parent(std::shared_ptr<Node>& parent);
  DataType get_type() const;
  StatementType get_st_type();
  std::shared_ptr<Node>& get_child (size_t node_num);
  size_t get_children_amount() const;
  std::shared_ptr<Node>& get_parent();
  void AddChild(std::shared_ptr<Node> const& node);
  static bool IsNodesEqual(const std::shared_ptr<Node>& node1,
                           const std::shared_ptr<Node>& node2);

  virtual void PrintData(std::ostream &stream) = 0;
  virtual void PrintType(std::ostream &stream) = 0;

private:
  void ValidateChildNumber(size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<Node> const& node) const;
  void ValidateStType(StatementType type);
  virtual void ValidateType(DataType type) const = 0;
};

class IntNumNode: public Node {
  int data_;
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  int get_data() const;
  void set_data(const int &data, DataType type);
  void set_data(const int &data);

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType(DataType type) const override;
};

class FloatNumNode: public Node {
  double data_;
public:
  explicit FloatNumNode(double value, DataType type = DataType::FLOAT_NUMBER);

  double get_data() const;
  void set_data(const double &data, DataType type);
  void set_data(const double &data);

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType(DataType type) const override;
};

class CharNode: public Node {
  char data_;
public:
  explicit CharNode(char ch, DataType type);

  char get_data() const;
  void set_data(const char &data, DataType type);
  void set_data(const char &data);


  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType(DataType type) const override;
};

class StringNode: public Node {
  std::string data_;
public:
  explicit StringNode(std::string string, DataType type);

  std::string get_data() const;
  void set_data(const std::string &data, DataType type);
  void set_data(const std::string &data);

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType(DataType type) const override;
};

class RootNode: public Node {
public:
  explicit RootNode();

  void PrintData(std::ostream &stream) override;
  void PrintType(std::ostream &stream) override;

private:
  void ValidateType(DataType type) const override;
};

namespace Tree {
  void PrintTreeRecursive(std::shared_ptr<Node> const &node,
                          std::ostream &stream);
};

//bool operator== (std::shared_ptr<Node> const& lhs, //TODO: decide how to implement operator== for shared_ptr<Node>
//                 std::shared_ptr<Node> const& rhs);
//
//bool operator!= (std::shared_ptr<Node> const& lhs,
//                 std::shared_ptr<Node> const& rhs);
