#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

constexpr std::string_view kST_NONE = "none";
constexpr std::string_view kST_PROGRAM = "program";
constexpr std::string_view kST_QUERY = "query";
constexpr std::string_view kST_DDL_STMT = "ddl";
constexpr std::string_view kST_DML_STMT = "dml";

constexpr std::string_view kST_ALTER_TABLE = "alter table";
constexpr std::string_view kST_CREATE_DATABASE = "create database";
constexpr std::string_view kST_CREATE_TABLE = "create table";
constexpr std::string_view kST_DROP_DATABASE = "drop database";
constexpr std::string_view kST_DROP_TABLE = "drop table";

constexpr std::string_view kST_TABLE_DEF = "table definition";
constexpr std::string_view kST_COLUMN_DEF = "column definition";
constexpr std::string_view kST_TABLE_CONSTRAINT = "table constraint";
constexpr std::string_view kST_ALTER_ACTION_ADD = "alter action add";
constexpr std::string_view kST_ALTER_ACTION_DROP = "alter action drop";
constexpr std::string_view kST_DROP_LIST = "drop list";
constexpr std::string_view kST_DROP_CONSTRAINT = "drop constraint";
constexpr std::string_view kST_DROP_COLUMN = "drop column";

constexpr std::string_view kST_DELETE = "delete";
constexpr std::string_view kST_INSERT = "insert";
constexpr std::string_view kST_UPDATE = "update";

constexpr std::string_view kST_CONDITION = "condition";
constexpr std::string_view kST_OR_CONDITION = "or condition";
constexpr std::string_view kST_AND_CONDITION = "and condition";
constexpr std::string_view kST_NOT_CONDITION = "not condition";
constexpr std::string_view kST_PREDICATE = "predicate";
constexpr std::string_view kST_EXPRESSION = "expression";

constexpr std::string_view kST_PRIMARY_KEY = "primary key";
constexpr std::string_view kST_FOREIGN_KEY = "foreign key";
constexpr std::string_view kST_REFERENCE = "reference";
constexpr std::string_view kST_NAME = "name";
constexpr std::string_view kST_IDENTIFIER = "identifier";
constexpr std::string_view kST_DOT_DELIMITER = "dot delimiter";
constexpr std::string_view kST_COMMA_DELIMITER = "comma delimiter";
constexpr std::string_view kST_SEMICOLON_DELIMITER = "semicolon delimiter";

constexpr std::string_view kST_CONSTRAINT_KW = "constraint";
constexpr std::string_view kST_COLUMN_KW = "column";
constexpr std::string_view kST_ADD_KW = "add";
constexpr std::string_view kST_DROP_KW = "drop";

constexpr std::string_view kST_INT_TYPE = "int";
constexpr std::string_view kST_INTEGER_TYPE = "integer";
constexpr std::string_view kST_FLOAT_TYPE = "float";
constexpr std::string_view kST_CHAR_TYPE = "char";
constexpr std::string_view kST_VARCHAR_TYPE = "varchar";

namespace scc::ast {

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

    // Other Keywords
    kConstraintKW,
    kColumnKW,
    kAddKW,
    kDropKW,

    // SQL Data Types
    kIntType,
    kFloatType,
    kCharType,
    kVarcharType,
  };

  StmtType() = default;
  StmtType(Value value);
  explicit StmtType(const std::string_view& str_stmt_type);

  std::string ToString() const;

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

std::ostream& operator<<(std::ostream& os, const StmtType& stmt_type);

} // scc::ast
