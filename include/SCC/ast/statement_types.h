#pragma once

enum StatementType {
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

  StTypeCount
};
