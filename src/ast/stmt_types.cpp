#include "SCC/ast/stmt_types.h"

namespace scc::ast {

StmtType::StmtType(Value value) {
  if (value > kVarcharType)
    throw std::invalid_argument("Incorrect value for Statement Type");
  this->value_ = value;
}
StmtType::StmtType(const std::string_view& str_stmt_type) {
  std::string stmt_type = scc::common::LowerCase(std::string(str_stmt_type));
  if (stmt_type == kST_NONE)
    this->value_ = kNone;
  else if (stmt_type == kST_PROGRAM)
    this->value_ = kProgram;
  else if (stmt_type == kST_QUERY)
    this->value_ = kQuery;
  else if (stmt_type == kST_DDL_STMT)
    this->value_ = kDdlStmt;
  else if (stmt_type == kST_DML_STMT)
    this->value_ = kDmlStmt;

  /* DDL Statements */
  else if (stmt_type == kST_ALTER_TABLE)
    this->value_ = kAlterTableStmt;
  else if (stmt_type == kST_CREATE_DATABASE)
    this->value_ = kCreateDatabaseStmt;
  else if (stmt_type == kST_CREATE_TABLE)
    this->value_ = kCreateTableStmt;
  else if (stmt_type == kST_DROP_DATABASE)
    this->value_ = kDropDatabaseStmt;
  else if (stmt_type == kST_DROP_TABLE)
    this->value_ = kDropTableStmt;

  /* DDL Basic Statements */
  else if (stmt_type == kST_TABLE_DEF)
    this->value_ = kTableDef;
  else if (stmt_type == kST_COLUMN_DEF)
    this->value_ = kColumnDef;
  else if (stmt_type == kST_TABLE_CONSTRAINT)
    this->value_ = kTableConstraint;
  else if (stmt_type == kST_ALTER_ACTION_ADD)
    this->value_ = kAlterActionAdd;
  else if (stmt_type == kST_ALTER_ACTION_DROP)
    this->value_ = kAlterActionDrop;
  else if (stmt_type == kST_DROP_LIST)
    this->value_ = kDropList;
  else if (stmt_type == kST_DROP_CONSTRAINT)
    this->value_ = kDropConstraint;
  else if (stmt_type == kST_DROP_COLUMN)
    this->value_ = kDropColumn;

  /* DML Statements */
  else if (stmt_type == kST_DELETE)
    this->value_ = kDeleteStmt;
  else if (stmt_type == kST_INSERT)
    this->value_ = kInsertStmt;
  else if (stmt_type == kST_UPDATE)
    this->value_ = kUpdateStmt;

  /* DML Basic Statements */
  else if (stmt_type == kST_CONDITION)
    this->value_ = kCondition;
  else if (stmt_type == kST_OR_CONDITION)
    this->value_ = kORCondition;
  else if (stmt_type == kST_AND_CONDITION)
    this->value_ = kANDCondition;
  else if (stmt_type == kST_NOT_CONDITION)
    this->value_ = kNOTCondition;
  else if (stmt_type == kST_PREDICATE)
    this->value_ = kPredicate;
  else if (stmt_type == kST_EXPRESSION)
    this->value_ = kExpression;

  /* Basic Statements */
  else if (stmt_type == kST_PRIMARY_KEY)
    this->value_ = kPrimaryKey;
  else if (stmt_type == kST_FOREIGN_KEY)
    this->value_ = kForeignKey;
  else if (stmt_type == kST_REFERENCE)
    this->value_ = kReference;
  else if (stmt_type == kST_NAME)
    this->value_ = kName;
  else if (stmt_type == kST_IDENTIFIER)
    this->value_ = kIdentifier;
  else if (stmt_type == kST_DOT_DELIMITER)
    this->value_ = kDotDelimiter;
  else if (stmt_type == kST_COMMA_DELIMITER)
    this->value_ = kCommaDelimiter;
  else if (stmt_type == kST_SEMICOLON_DELIMITER)
    this->value_ = kSemicolonDelimiter;

  /* Other keywords */
  else if (stmt_type == kST_CONSTRAINT_KW)
    this->value_ = kConstraintKW;

  /* SQL Data Types */
  else if (stmt_type == kST_INT_TYPE || stmt_type == kST_INTEGER_TYPE)
    this->value_ = kIntType;
  else if (stmt_type == kST_FLOAT_TYPE)
    this->value_ = kFloatType;
  else if (stmt_type == kST_CHAR_TYPE)
    this->value_ = kCharType;
  else if (stmt_type == kST_VARCHAR_TYPE)
    this->value_ = kVarcharType;
  else
    throw std::invalid_argument("No Statement Type for '" + std::string(str_stmt_type) + "'");
}

std::string StmtType::ToString() const {
  switch (value_) {
    case kNone:
      return std::string(kST_NONE);
    case kProgram:
      return std::string(kST_PROGRAM);
    case kQuery:
      return std::string(kST_QUERY);
    case kDdlStmt:
      return std::string(kST_DDL_STMT);
    case kDmlStmt:
      return std::string(kST_DML_STMT);

    /* DDL Statements */
    case kAlterTableStmt:
      return std::string(kST_ALTER_TABLE);
    case kCreateDatabaseStmt:
      return std::string(kST_CREATE_DATABASE);
    case kCreateTableStmt:
      return std::string(kST_CREATE_TABLE);
    case kDropDatabaseStmt:
      return std::string(kST_DROP_DATABASE);
    case kDropTableStmt:
      return std::string(kST_DROP_TABLE);

    /* DDL Basic Statements */
    case kTableDef:
      return std::string(kST_TABLE_DEF);
    case kColumnDef:
      return std::string(kST_COLUMN_DEF);
    case kTableConstraint:
      return std::string(kST_TABLE_CONSTRAINT);
    case kAlterActionAdd:
      return std::string(kST_ALTER_ACTION_ADD);
    case kAlterActionDrop:
      return std::string(kST_ALTER_ACTION_DROP);
    case kDropList:
      return std::string(kST_DROP_LIST);
    case kDropConstraint:
      return std::string(kST_DROP_CONSTRAINT);
    case kDropColumn:
      return std::string(kST_DROP_COLUMN);

    /* DML Statements */
    case kDeleteStmt:
      return std::string(kST_DELETE);
    case kInsertStmt:
      return std::string(kST_INSERT);
    case kUpdateStmt:
      return std::string(kST_UPDATE);

    /* DML Basic Statements */
    case kCondition:
      return std::string(kST_CONDITION);
    case kORCondition:
      return std::string(kST_OR_CONDITION);
    case kANDCondition:
      return std::string(kST_AND_CONDITION);
    case kNOTCondition:
      return std::string(kST_NOT_CONDITION);
    case kPredicate:
      return std::string(kST_PREDICATE);
    case kExpression:
      return std::string(kST_EXPRESSION);

    /* Basic Statements */
    case kPrimaryKey:
      return std::string(kST_PRIMARY_KEY);
    case kForeignKey:
      return std::string(kST_FOREIGN_KEY);
    case kReference:
      return std::string(kST_REFERENCE);
    case kName:
      return std::string(kST_NAME);
    case kIdentifier:
      return std::string(kST_IDENTIFIER);
    case kDotDelimiter:
      return std::string(kST_DOT_DELIMITER);
    case kCommaDelimiter:
      return std::string(kST_COMMA_DELIMITER);
    case kSemicolonDelimiter:
      return std::string(kST_SEMICOLON_DELIMITER);

    /* Other Keywords */
    case kConstraintKW:
      return std::string(kST_CONSTRAINT_KW);

    /* SQL Data Types */
    case kIntType:
      return std::string(kST_INT_TYPE);
    case kFloatType:
      return std::string(kST_FLOAT_TYPE);
    case kCharType:
      return std::string(kST_CHAR_TYPE);
    case kVarcharType:
      return std::string(kST_VARCHAR_TYPE);
  }
}

std::ostream& operator<<(std::ostream& os, const StmtType& stmt_type) {
  os << stmt_type.ToString();
  return os;
}

} // scc::ast
