/**
 * \file
 * \brief Header file for AST nodes classes
 *
 * \details This file contains some required enums, the base node class and
 * derived node classes representing different data types.
 */

#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "SCC/config/config.h"

#include "logger/log.hpp"

/**
 * \brief Set of SQL statement types for the AST
 *
 * \details This enum contains all SQL statement types, that available to
 * translation
 */
enum StatementType {
  EMPTY_TYPE,               ///< Type for not statement node
  Program,                  ///< The ROOT of AST
  query,                    ///< The single query
  ddlStatement,             ///< DDL query
  dmlStatement,             ///< DML query

  // DDL Statements
  alterTableStatement,      ///< CREATE TABLE statement
  createDatabaseStatement,  ///< CREATE DATABASE statement
  createTableStatement,     ///< CREATE TABLE statement
  dropDatabaseStatement,    ///< DROP DATABASE statement
  dropTableStatement,       ///< DROP TABLE statement

  // DDL Basic Statements
  tableDefinition,          ///< Start of table definition
  columnDefinition,         ///< Start of column definition
  tableConstraint,          ///< Start of table constraint
  alterActionADD,           ///< ADD statement
  alterActionDROP,          ///< DROP statement
  dropList,                 ///< List of drop statements
  dropConstraint,           ///< Drop constraint statement
  dropColumn,               ///< Drop column statement

  // DML Statements
  deleteStatement,          ///< DELETE statement
  insertStatement,          ///< INSERT statement
  updateStatement,          ///< UPDATE statement

  // DML Basic Statements
  condition,                ///< Statement for condition
  ORcondition,              ///< Statement for OR condition
  ANDcondition,             ///< Statement for AND condition
  NOTcondition,             ///< Statement for NOT condition
  predicate,                ///< Statement for predicate
  expression,               ///< Statement for logical expression

  // Basic Statements
  primaryKey,               ///< PRIMARY KEY definition
  foreignKey,               ///< FOREIGN KEY definition
  reference,                ///< Reference for foreign key
  name,                     ///< Name, consisting of identifiers separated by delimiter dots
  identifier,               ///< Part of name
  delimiter_dot,            ///< Delimiter dot (.)
  delimiter_comma,          ///< Delimiter comma (,)
  delimiter_semicolon,      ///< Delimiter semicolon (;)

  // Other key words
  kw_constraint,            ///< CONSTRAINT key word

  // SQL datatypes
  SQL_int,                  ///< Integer SQL data type
  SQL_float,                ///< Float SQL data type
  SQL_char,                 /// Char SQL data type
  SQL_varchar,              ///< Varchar SQL data type

  StTypeCount               ///<
};

/**
 * \brief Set of data types for, that AST nodes can store
 */
enum DataType {
  ROOT,         ///< Data type for the root of AST
  SERVICE,      ///< DATA type for service nodes (without data)
  INT_NUMBER,   ///< Int data type
  FLOAT_NUMBER, ///< Float data type
  BRACKET,      ///< Char data type for brackets
  PUNCTUATION,  ///< String data type for punctuations
  OPERATOR,     ///< String data type for operators
  WORD,         ///< String data type for words
  STRING        ///< String data type for string data from queries
};

/**
 * \brief Node base class
 *
 * \details This class is the base class for all nodes. Here all shared methods
 * are implemented.
 */
class Node {
public:
  /**
   * \brief Constructor for base node
   *
   * \details Sets the "type_" field with data type for node.
   *
   * @param[in] type node data type
   */
  explicit Node(DataType type);
  virtual ~Node();

  /**
   * \brief Sets line
   *
   * \details Sets the "line_" field with line where terminal symbol, from which node was created,
   * was found.
   *
   * @param[in] line line where terminal symbol, from which node was created,
   * was found.
   */
  void set_line(int line);
  /**
   * \brief Gets line
   *
   * \details Gets the "line_" field with line where terminal symbol, from which
   * node was created, was found.
   *
   * @return line where terminal symbol, from which node was created, was found.
   */
  int get_line();
  /**
   * \brief Sets statement type for node
   *
   * \details Validate statement type and than sets "st_type_" field with this
   * type.
   *
   * @param[in] type statement type for node
   */
  void set_st_type(StatementType type);
  /**
   * \brief Gets node statement type
   *
   * \details Gets the "st_type_" field with node statement type.
   *
   * @return statement type
   */
  StatementType get_st_type();
  /**
   * \brief Sets parent node
   *
   * \details Sets "parent_" field with pointer to parent of current node.
   *
   * @param[in] parent pointer(shared_ptr<Node>) to parent
   */
  void set_parent(std::shared_ptr<Node>& parent);
  /**
   * \brief Gets node data type
   *
   * \details Gets the "type_" field with node data type.
   *
   * @return data type
   */
  DataType get_type() const;
  /**
   * \brief Gets one of the nodes child
   *
   * \details Gets one element from the vector field "children_". The number of
   * element passes by parameter. Also validate passing parameter.
   *
   * @param node_num number of child node
   *
   * @return required child node
   */
  std::shared_ptr<Node>& get_child(size_t node_num);
  /**
   * \brief Gets children amount
   *
   * \details Gets number of elements in vector "children_".
   *
   * @return children amount
   */
  size_t get_children_amount() const;
  /**
   * \brief Gets pointer to parent
   *
   * \details Gets the "parent_" field, that contains pointer(shared_ptr<Node>)
   * to parent node.
   *
   * @return pointer(shared_ptr<Node>) to parent
   */
  std::shared_ptr<Node>& get_parent();

  /**
   * \brief Add child to current node
   *
   * \details Validate is passing node not nullptr and push it into "children_"
   * vector if it is not.
   *
   * @param[in] parent pointer(shared_ptr<Node>) to child node
   */
  void AddChild(std::shared_ptr<Node> const& node);
  /**
   * \brief Compare two nodes
   *
   * \details Compare two nodes, by parameters like: type, statement type, data,
   * children amount, parent and line. Recursively goes through all nodes
   * children and compare them. If there is some differences, returns false.
   * Else returns true.
   *
   * @param[in] node1 pointer(shared_ptr<Node>) to first node
   * @param[in] node2  pointer(shared_ptr<Node>) to second node
   *
   * @return Is nodes equals
   */
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
  /**
   * \brief Validate child number
   *
   * \details Check is passing number bigger than children amount in current
   * node. If it is bigger, exits with EXIT_FAILURE.
   *
   * @param[in] node_num children number
   *
   */
  void ValidateChildNumber(size_t node_num) const;
  /**
   * \brief Validate is passing node is nullptr
   *
   * \details Check is passing node is passing node is nullptr. If it is, exits
   * with EXIT_FAILURE.
   *
   * @param[in] node pointer to child node.
   *
   */
  void ValidateAddChild(std::shared_ptr<Node> const& node) const;
  /**
   * \brief Check is statement type valid
   *
   * @param[in] type statement type to check
   *
   */
  void ValidateStType(StatementType type);
  /**
   * \brief Check is data type valid
   *
   * @param[in] type data type to check
   *
   */
  virtual void ValidateType(DataType type) const = 0;
};

/**
 * \brief Node class for integer data
 *
 * \details This class contain data field and methods, implemented for integer
 * data
 */
class IntNumNode : public Node {
public:
  /**
   * \brief Constructor for node with integer data
   *
   * \details Sets the "type_" field with data type for node and "data_" field
   *
   * @param[in] value value for "data_" field
   * @param[in] type node data type
   */
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  /**
   * \brief Sets the "data_" and "type_" field
   *
   * @param[in] value value for "data_" field
   * @param[in] type node data type
   */
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
