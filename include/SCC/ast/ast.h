#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "SCC/ast/data_types.h"
#include "SCC/ast/statement_types.h"
#include "SCC/config/scc_config.h"

#include "logger/log.hpp"

class Node {
public:
  explicit Node(DataType type);
  virtual ~Node();

  void set_line(int line);
  int get_line();
  void set_st_type(StatementType type);
  StatementType get_st_type();
  void set_parent(std::shared_ptr<Node>& parent);
  std::shared_ptr<Node>& get_parent();
  DataType get_type() const;
  std::shared_ptr<Node>& get_child(size_t node_num);
  size_t get_children_amount() const;

  void AddChild(std::shared_ptr<Node> const& node);
  static bool IsNodesEqual(const std::shared_ptr<Node>& node1,
                           const std::shared_ptr<Node>& node2);

  virtual void PrintData(std::ostream& stream) = 0;
  virtual void PrintType(std::ostream& stream) = 0;

protected:
  int line_ = 0;
  DataType type_;
  StatementType st_type_ = StatementType::EMPTY_TYPE;
  std::shared_ptr<Node> parent_ = nullptr;
  std::vector<std::shared_ptr<Node>> children_;

private:
  void ValidateChildNumber(size_t node_num) const;
  void ValidateAddChild(std::shared_ptr<Node> const& node) const;
  void ValidateStType(StatementType type);
  virtual void ValidateType(DataType type) const = 0;
};

class IntNumNode : public Node {
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  void set_data(const int& data, DataType type);
  void set_data(const int& data);
  int get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  int data_;

  void ValidateType(DataType type) const override;
};

class FloatNumNode : public Node {
public:
  explicit FloatNumNode(double value, DataType type = DataType::FLOAT_NUMBER);

  void set_data(const double& data, DataType type);
  void set_data(const double& data);
  double get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  double data_;

  void ValidateType(DataType type) const override;
};

class CharNode : public Node {
public:
  explicit CharNode(char ch, DataType type);

  void set_data(const char& data, DataType type);
  void set_data(const char& data);
  char get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  char data_;

  void ValidateType(DataType type) const override;
};

class StringNode : public Node {
public:
  explicit StringNode(std::string string, DataType type);

  void set_data(const std::string& data, DataType type);
  void set_data(const std::string& data);
  std::string get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  std::string data_;

  void ValidateType(DataType type) const override;
};

class RootNode : public Node {
public:
  explicit RootNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  void ValidateType(DataType type) const override;
};

class ServiceNode : public Node {
public:
  explicit ServiceNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  void ValidateType(DataType type) const override;
};

namespace Tree {
void PrintTreeRecursive(std::shared_ptr<Node> const& node,
                        std::ostream& stream);
};
