#pragma once

#include <iostream>
#include <stdexcept>

class StmtType {
public:
  enum Value : unsigned char {
    kNone,
    kProgram,
    kQuery,
    kDdlStmt,
    kDmlStmt,

    // DDL Statements
    kAlterTableStmt,
    kCreateDatabaseStmt,
    kCreateTableStmt,
    kDropDatabaseStmt,
    kDropTableStmt,

    // DDL Basic Statements
    kTableDef,
    kColumnDef,
    kTableConstraint,
    kAlterActionAdd,
    kAlterActionDrop,
    kDropList,
    kDropConstraint,
    kDropColumn,

    // DML Statements
    kDeleteStmt,
    kInsertStmt,
    kUpdateStmt,

    // DML Basic Statements
    kCondition,
    kORCondition,
    kANDCondition,
    kNOTCondition,
    kPredicate,
    kExpression,

    // Basic Statements
    kPrimaryKey,
    kForeignKey,
    kReference,
    kName,
    kIdentifier,
    kDotDelimiter,
    kCommaDelimiter,
    kSemicolonDelimiter,

    // Other key words
    kConstraintKW,

    // SQL data types
    kIntType,
    kFloatType,
    kCharType,
    kVarcharType,
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
  Value value_ = Value::kNone;
};

//std::ostream& operator<<(std::ostream& os, const StmtType& stmt_type);
