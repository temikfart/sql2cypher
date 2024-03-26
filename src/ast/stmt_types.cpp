#include "SCC/ast/stmt_types.h"

namespace scc::ast {

StmtType::StmtType(Value value) {
  if (value > kVarcharType)
    throw std::invalid_argument("Incorrect value for Statement Type");
  this->value_ = value;
}
StmtType::StmtType(const std::string& str_stmt_type) {
  std::string stmt_type = scc::common::LowerCase(str_stmt_type);
  if (stmt_type == ST_NONE)
    this->value_ = kNone;
  else if (stmt_type == ST_PROGRAM)
    this->value_ = kProgram;
  else if (stmt_type == ST_QUERY)
    this->value_ = kQuery;
  else if (stmt_type == ST_DDL_STMT)
    this->value_ = kDdlStmt;
  else if (stmt_type == ST_DML_STMT)
    this->value_ = kDmlStmt;

  /* DDL Statements */
  else if (stmt_type == ST_ALTER_TABLE)
    this->value_ = kAlterTableStmt;
  else if (stmt_type == ST_CREATE_DATABASE)
    this->value_ = kCreateDatabaseStmt;
  else if (stmt_type == ST_CREATE_TABLE)
    this->value_ = kCreateTableStmt;
  else if (stmt_type == ST_DROP_DATABASE)
    this->value_ = kDropDatabaseStmt;
  else if (stmt_type == ST_DROP_TABLE)
    this->value_ = kDropTableStmt;

  /* DDL Basic Statements */
  else if (stmt_type == ST_TABLE_DEF)
    this->value_ = kTableDef;
  else if (stmt_type == ST_COLUMN_DEF)
    this->value_ = kColumnDef;
  else if (stmt_type == ST_TABLE_CONSTRAINT)
    this->value_ = kTableConstraint;
  else if (stmt_type == ST_ALTER_ACTION_ADD)
    this->value_ = kAlterActionAdd;
  else if (stmt_type == ST_ALTER_ACTION_DROP)
    this->value_ = kAlterActionDrop;
  else if (stmt_type == ST_DROP_LIST)
    this->value_ = kDropList;
  else if (stmt_type == ST_DROP_CONSTRAINT)
    this->value_ = kDropConstraint;
  else if (stmt_type == ST_DROP_COLUMN)
    this->value_ = kDropColumn;

  /* DML Statements */
  else if (stmt_type == ST_DELETE)
    this->value_ = kDeleteStmt;
  else if (stmt_type == ST_INSERT)
    this->value_ = kInsertStmt;
  else if (stmt_type == ST_UPDATE)
    this->value_ = kUpdateStmt;

  /* DML Basic Statements */
  else if (stmt_type == ST_CONDITION)
    this->value_ = kCondition;
  else if (stmt_type == ST_OR_CONDITION)
    this->value_ = kORCondition;
  else if (stmt_type == ST_AND_CONDITION)
    this->value_ = kANDCondition;
  else if (stmt_type == ST_NOT_CONDITION)
    this->value_ = kNOTCondition;
  else if (stmt_type == ST_PREDICATE)
    this->value_ = kPredicate;
  else if (stmt_type == ST_EXPRESSION)
    this->value_ = kExpression;

  /* Basic Statements */
  else if (stmt_type == ST_PRIMARY_KEY)
    this->value_ = kPrimaryKey;
  else if (stmt_type == ST_FOREIGN_KEY)
    this->value_ = kForeignKey;
  else if (stmt_type == ST_REFERENCE)
    this->value_ = kReference;
  else if (stmt_type == ST_NAME)
    this->value_ = kName;
  else if (stmt_type == ST_IDENTIFIER)
    this->value_ = kIdentifier;
  else if (stmt_type == ST_DOT_DELIMITER)
    this->value_ = kDotDelimiter;
  else if (stmt_type == ST_COMMA_DELIMITER)
    this->value_ = kCommaDelimiter;
  else if (stmt_type == ST_SEMICOLON_DELIMITER)
    this->value_ = kSemicolonDelimiter;

  /* Other keywords */
  else if (stmt_type == ST_CONSTRAINT_KW)
    this->value_ = kConstraintKW;

  /* SQL Data Types */
  else if (stmt_type == ST_INT_TYPE)
    this->value_ = kIntType;
  else if (stmt_type == ST_FLOAT_TYPE)
    this->value_ = kFloatType;
  else if (stmt_type == ST_CHAR_TYPE)
    this->value_ = kCharType;
  else if (stmt_type == ST_VARCHAR_TYPE)
    this->value_ = kVarcharType;
  else
    throw std::invalid_argument("No Statement Type for '" + str_stmt_type + "'");
}

std::string StmtType::ToString() const {
  switch (value_) {
    case kNone:
      return ST_NONE;
    case kProgram:
      return ST_PROGRAM;
    case kQuery:
      return ST_QUERY;
    case kDdlStmt:
      return ST_DDL_STMT;
    case kDmlStmt:
      return ST_DML_STMT;

    /* DDL Statements */
    case kAlterTableStmt:
      return ST_ALTER_TABLE;
    case kCreateDatabaseStmt:
      return ST_CREATE_DATABASE;
    case kCreateTableStmt:
      return ST_CREATE_TABLE;
    case kDropDatabaseStmt:
      return ST_DROP_DATABASE;
    case kDropTableStmt:
      return ST_DROP_TABLE;

    /* DDL Basic Statements */
    case kTableDef:
      return ST_TABLE_DEF;
    case kColumnDef:
      return ST_COLUMN_DEF;
    case kTableConstraint:
      return ST_TABLE_CONSTRAINT;
    case kAlterActionAdd:
      return ST_ALTER_ACTION_ADD;
    case kAlterActionDrop:
      return ST_ALTER_ACTION_DROP;
    case kDropList:
      return ST_DROP_LIST;
    case kDropConstraint:
      return ST_DROP_CONSTRAINT;
    case kDropColumn:
      return ST_DROP_COLUMN;

    /* DML Statements */
    case kDeleteStmt:
      return ST_DELETE;
    case kInsertStmt:
      return ST_INSERT;
    case kUpdateStmt:
      return ST_UPDATE;

    /* DML Basic Statements */
    case kCondition:
      return ST_CONDITION;
    case kORCondition:
      return ST_OR_CONDITION;
    case kANDCondition:
      return ST_AND_CONDITION;
    case kNOTCondition:
      return ST_NOT_CONDITION;
    case kPredicate:
      return ST_PREDICATE;
    case kExpression:
      return ST_EXPRESSION;

    /* Basic Statements */
    case kPrimaryKey:
      return ST_PRIMARY_KEY;
    case kForeignKey:
      return ST_FOREIGN_KEY;
    case kReference:
      return ST_REFERENCE;
    case kName:
      return ST_NAME;
    case kIdentifier:
      return ST_IDENTIFIER;
    case kDotDelimiter:
      return ST_DOT_DELIMITER;
    case kCommaDelimiter:
      return ST_COMMA_DELIMITER;
    case kSemicolonDelimiter:
      return ST_SEMICOLON_DELIMITER;

    /* Other Keywords */
    case kConstraintKW:
      return ST_CONSTRAINT_KW;

    /* SQL Data Types */
    case kIntType:
      return ST_INT_TYPE;
    case kFloatType:
      return ST_FLOAT_TYPE;
    case kCharType:
      return ST_CHAR_TYPE;
    case kVarcharType:
      return ST_VARCHAR_TYPE;
  }
}

std::ostream& operator<<(std::ostream& os, const StmtType& stmt_type) {
  os << stmt_type.ToString();
  return os;
}

} // scc::ast
