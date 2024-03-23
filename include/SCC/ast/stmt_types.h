#pragma once

#include <iostream>
#include <stdexcept>

class StmtType {
public:
  enum Value : unsigned char {
    EMPTY_TYPE,
    Program,
    query,
    ddlStatement,
    dmlStatement,

    // DDL Statements
    alterTableStatement,
    createDatabaseStatement,
    createTableStatement,
    dropDatabaseStatement,
    dropTableStatement,

    // DDL Basic Statements
    tableDefinition,
    columnDefinition,
    tableConstraint,
    alterActionADD,
    alterActionDROP,
    dropList,
    dropConstraint,
    dropColumn,

    // DML Statements
    deleteStatement,
    insertStatement,
    updateStatement,

    // DML Basic Statements
    condition,
    ORcondition,
    ANDcondition,
    NOTcondition,
    predicate,
    expression,

    // Basic Statements
    primaryKey,
    foreignKey,
    reference,
    name,
    identifier,
    delimiter_dot,
    delimiter_comma,
    delimiter_semicolon,

    // Other key words
    kw_constraint,

    // SQL datatypes
    SQL_int,
    SQL_float,
    SQL_char,
    SQL_varchar,
  };

  StmtType() = default;
  StmtType(Value value);
//  explicit StmtType(const std::string& stmt_type);

//  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const StmtType& mode) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::EMPTY_TYPE;
};

//std::ostream& operator<<(std::ostream& os, const StmtType& stmt_type);
