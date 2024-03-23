#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

#define ST_NONE "none"
#define ST_PROGRAM "program"
#define ST_QUERY "query"
#define ST_DDL_STMT "ddl"
#define ST_DML_STMT "dml"

#define ST_ALTER_TABLE "alter table"
#define ST_CREATE_DATABASE "create database"
#define ST_CREATE_TABLE "create table"
#define ST_DROP_DATABASE "drop database"
#define ST_DROP_TABLE "drop table"

#define ST_TABLE_DEF "table definition"
#define ST_COLUMN_DEF "column definition"
#define ST_TABLE_CONSTRAINT "table constraint"
#define ST_ALTER_ACTION_ADD "alter action add"
#define ST_ALTER_ACTION_DROP "alter action drop"
#define ST_DROP_LIST "drop list"
#define ST_DROP_CONSTRAINT "drop constraint"
#define ST_DROP_COLUMN "drop column"

#define ST_DELETE "delete"
#define ST_INSERT "insert"
#define ST_UPDATE "update"

#define ST_CONDITION "condition"
#define ST_OR_CONDITION "or condition"
#define ST_AND_CONDITION "and condition"
#define ST_NOT_CONDITION "not condition"
#define ST_PREDICATE "predicate"
#define ST_EXPRESSION "expression"

#define ST_PRIMARY_KEY "primary Key"
#define ST_FOREIGN_KEY "foreign Key"
#define ST_REFERENCE "reference"
#define ST_NAME "name"
#define ST_IDENTIFIER "identifier"
#define ST_DOT_DELIMITER "dot delimiter"
#define ST_COMMA_DELIMITER "comma delimiter"
#define ST_SEMICOLON_DELIMITER "semicolon delimiter"

#define ST_CONSTRAINT_KW "constraint kw"

#define ST_INT_TYPE "int"
#define ST_FLOAT_TYPE "float"
#define ST_CHAR_TYPE "char"
#define ST_VARCHAR_TYPE "varchar"

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

    // SQL Data Types
    kIntType,
    kFloatType,
    kCharType,
    kVarcharType,
  };

  StmtType() = default;
  StmtType(Value value);
  explicit StmtType(const std::string& str_stmt_type);

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
