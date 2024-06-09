#include "SCC/ast/stmt_types.h"

namespace scc::ast {

StmtType::StmtType(Value value) {
  if (value > kUnknownValue)
    throw std::invalid_argument("Incorrect value for Statement Type");
  this->value_ = value;
}
StmtType::StmtType(const std::string_view& str_type) {
  std::string type = scc::common::LowerCase(std::string(str_type));
  if (type == kST_NONE)
    this->value_ = kNone;
  else if (type == kST_PROGRAM)
    this->value_ = kProgram;
  else if (type == kST_QUERY)
    this->value_ = kQuery;
  else if (type == kST_DDL_STMT)
    this->value_ = kDdlStmt;
  else if (type == kST_DML_STMT)
    this->value_ = kDmlStmt;

  /* DDL Statements */
  else if (type == kST_ALTER_TABLE)
    this->value_ = kAlterTableStmt;
  else if (type == kST_CREATE_DATABASE)
    this->value_ = kCreateDatabaseStmt;
  else if (type == kST_CREATE_TABLE)
    this->value_ = kCreateTableStmt;
  else if (type == kST_DROP_DATABASE)
    this->value_ = kDropDatabaseStmt;
  else if (type == kST_DROP_TABLE)
    this->value_ = kDropTableStmt;

  /* DDL Basic Statements */
  else if (type == kST_TABLE_DEF)
    this->value_ = kTableDef;
  else if (type == kST_COLUMN_DEF)
    this->value_ = kColumnDef;
  else if (type == kST_TABLE_CONSTRAINT)
    this->value_ = kTableConstraint;
  else if (type == kST_ALTER_ACTION_ADD)
    this->value_ = kAlterActionAdd;
  else if (type == kST_ALTER_ADD_LIST)
    this->value_ = kAlterAddList;
  else if (type == kST_ALTER_ACTION_DROP)
    this->value_ = kAlterActionDrop;
  else if (type == kST_ALTER_DROP_LIST)
    this->value_ = kAlterDropList;
  else if (type == kST_DROP_CONSTRAINT)
    this->value_ = kDropConstraint;
  else if (type == kST_DROP_COLUMN)
    this->value_ = kDropColumn;

  /* DML Statements */
  else if (type == kST_DELETE)
    this->value_ = kDeleteStmt;
  else if (type == kST_INSERT)
    this->value_ = kInsertStmt;
  else if (type == kST_UPDATE)
    this->value_ = kUpdateStmt;

  /* DML Basic Statements */
  else if (type == kST_UPDATE_COLUMN)
    this->value_ = kUpdateColumn;

  else if (type == kST_CONDITION)
    this->value_ = kCondition;
  else if (type == kST_OR_CONDITION)
    this->value_ = kORCondition;
  else if (type == kST_AND_CONDITION)
    this->value_ = kANDCondition;
  else if (type == kST_NOT_CONDITION)
    this->value_ = kNOTCondition;
  else if (type == kST_PREDICATE)
    this->value_ = kPredicate;
  else if (type == kST_EXPRESSION)
    this->value_ = kExpression;

  else if (type == kST_MATH_EXPRESSION)
    this->value_ = kMathExpression;
  else if (type == kST_SUM)
    this->value_ = kSum;
  else if (type == kST_PRODUCT)
    this->value_ = kProduct;
  else if (type == kST_POWER)
    this->value_ = kPower;
  else if (type == kST_VALUE)
    this->value_ = kValue;

  /* Logical Operators */
  else if (type == kST_OR_OPERATOR)
    this->value_ = kOROperator;
  else if (type == kST_AND_OPERATOR)
    this->value_ = kANDOperator;
  else if (type == kST_NOT_OPERATOR)
    this->value_ = kNOTOperator;

  /* Basic Statements */
  else if (type == kST_PRIMARY_KEY)
    this->value_ = kPrimaryKey;
  else if (type == kST_FOREIGN_KEY)
    this->value_ = kForeignKey;
  else if (type == kST_IDENTIFIER)
    this->value_ = kIdentifier;
  else if (type == kST_DOT_DELIMITER)
    this->value_ = kDotDelimiter;
  else if (type == kST_COMMA_DELIMITER)
    this->value_ = kCommaDelimiter;
  else if (type == kST_SEMICOLON_DELIMITER)
    this->value_ = kSemicolonDelimiter;

  /* Other keywords */
  else if (type == kST_CONSTRAINT_KW)
    this->value_ = kConstraintKW;
  else if (type == kST_COLUMN_KW)
    this->value_ = kColumnKW;
  else if (type == kST_REFERENCES_KW)
    this->value_ = kReferencesKW;
  else if (type == kST_ADD_KW)
    this->value_ = kAddKW;
  else if (type == kST_DROP_KW)
    this->value_ = kDropKW;
  else if (type == kST_INTO_KW)
    this->value_ = kIntoKW;
  else if (type == kST_SET_KW)
    this->value_ = kSetKW;
  else if (type == kST_FROM_KW)
    this->value_ = kFromKW;
  else if (type == kST_WHERE_KW)
    this->value_ = kWhereKW;
  else if (type == kST_VALUES_KW)
    this->value_ = kValuesKW;

  /* SQL Data Types */
  else if (type == kST_INT_TYPE || type == kST_INTEGER_TYPE)
    this->value_ = kIntType;
  else if (type == kST_FLOAT_TYPE)
    this->value_ = kFloatType;
  else if (type == kST_CHAR_TYPE)
    this->value_ = kCharType;
  else if (type == kST_VARCHAR_TYPE)
    this->value_ = kVarcharType;

  /* Special values */
  else if (type == kST_NULL_VALUE)
    this->value_ = kNullValue;
  else if (type == kST_UNKNOWN_VALUE)
    this->value_ = kUnknownValue;

  else
    throw std::invalid_argument("No Statement Type for '" + std::string(str_type) + "'");
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
    case kAlterAddList:
      return std::string(kST_ALTER_ADD_LIST);
    case kAlterActionDrop:
      return std::string(kST_ALTER_ACTION_DROP);
    case kAlterDropList:
      return std::string(kST_ALTER_DROP_LIST);
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
    case kUpdateColumn:
      return std::string(kST_UPDATE_COLUMN);

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

    case kMathExpression:
      return std::string(kST_MATH_EXPRESSION);
    case kSum:
      return std::string(kST_SUM);
    case kProduct:
      return std::string(kST_PRODUCT);
    case kPower:
      return std::string(kST_POWER);
    case kValue:
      return std::string(kST_VALUE);

    /* Logical Operators */
    case kOROperator:
      return std::string(kST_OR_OPERATOR);
    case kANDOperator:
      return std::string(kST_AND_OPERATOR);
    case kNOTOperator:
      return std::string(kST_NOT_OPERATOR);

    /* Basic Statements */
    case kPrimaryKey:
      return std::string(kST_PRIMARY_KEY);
    case kForeignKey:
      return std::string(kST_FOREIGN_KEY);
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
    case kColumnKW:
      return std::string(kST_COLUMN_KW);
    case kReferencesKW:
      return std::string(kST_REFERENCES_KW);
    case kAddKW:
      return std::string(kST_ADD_KW);
    case kDropKW:
      return std::string(kST_DROP_KW);
    case kIntoKW:
      return std::string(kST_INTO_KW);
    case kSetKW:
      return std::string(kST_SET_KW);
    case kFromKW:
      return std::string(kST_FROM_KW);
    case kWhereKW:
      return std::string(kST_WHERE_KW);
    case kValuesKW:
      return std::string(kST_VALUES_KW);

    /* SQL Data Types */
    case kIntType:
      return std::string(kST_INT_TYPE);
    case kFloatType:
      return std::string(kST_FLOAT_TYPE);
    case kCharType:
      return std::string(kST_CHAR_TYPE);
    case kVarcharType:
      return std::string(kST_VARCHAR_TYPE);

    case kNullValue:
      return std::string(kST_NULL_VALUE);
    case kUnknownValue:
      return std::string(kST_UNKNOWN_VALUE);
  }
}

std::ostream& operator<<(std::ostream& os, const StmtType& type) {
  os << type.ToString();
  return os;
}

} // scc::ast
