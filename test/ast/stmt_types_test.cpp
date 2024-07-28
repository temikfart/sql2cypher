#include <compare>

#include "gtest/gtest.h"

#include "SCC/ast/stmt_types.h"
#include "SCC/fixtures_common/enum.h"

using namespace scc::ast;
using namespace testing;

using UChar = unsigned char;

TEST(StmtTypeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, StmtType::Value>(0);
  CastEnumTestBody<UChar, StmtType::Value>(-1);
  CastEnumTestBody<UChar, StmtType::Value>(50);
  CastEnumTestBody<UChar, StmtType::Value>(128);
  CastEnumTestBody<UChar, StmtType::Value>(256);
}

TEST(StmtTypeCtorTests, DefaultValueTest) {
  StmtType type;
  EXPECT_EQ(StmtType::kNone, (StmtType::Value) type);
}

TEST(StmtTypeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<StmtType, StmtType::Value>(StmtType::kNone);
  CastWrapperToValueTestBody<StmtType, StmtType::Value>(StmtType::kCreateDatabaseStmt);
  CastWrapperToValueTestBody<StmtType, StmtType::Value>(StmtType::kAlterAddList);
  CastWrapperToValueTestBody<StmtType, StmtType::Value>(StmtType::kUpdateColumn);
  CastWrapperToValueTestBody<StmtType, StmtType::Value>(StmtType::kName);
}

TEST(StmtTypeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<StmtType, StmtType::Value>(-1);
  CreateWithInvalidArgumentTestBody<StmtType, StmtType::Value>(60);
}

TEST(StmtTypeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<StmtType, StmtType::Value>("09a8sdg");
  CreateWithInvalidArgumentTestBody<StmtType, StmtType::Value>("n0ne");
  CreateWithInvalidArgumentTestBody<StmtType, StmtType::Value>("updates");
}

TEST(StmtTypeCtorTests, StringValueTest) {
  CreateFromStringTestBody<StmtType>(kST_NONE, StmtType::kNone);
  CreateFromStringTestBody<StmtType>(kST_PROGRAM, StmtType::kProgram);
  CreateFromStringTestBody<StmtType>(kST_QUERY, StmtType::kQuery);
  CreateFromStringTestBody<StmtType>(kST_DDL_STMT, StmtType::kDdlStmt);
  CreateFromStringTestBody<StmtType>(kST_DML_STMT, StmtType::kDmlStmt);

  CreateFromStringTestBody<StmtType>(kST_ALTER_TABLE, StmtType::kAlterTableStmt);
  CreateFromStringTestBody<StmtType>(kST_CREATE_DATABASE, StmtType::kCreateDatabaseStmt);
  CreateFromStringTestBody<StmtType>(kST_CREATE_TABLE, StmtType::kCreateTableStmt);
  CreateFromStringTestBody<StmtType>(kST_DROP_DATABASE, StmtType::kDropDatabaseStmt);
  CreateFromStringTestBody<StmtType>(kST_DROP_TABLE, StmtType::kDropTableStmt);

  CreateFromStringTestBody<StmtType>(kST_TABLE_DEF, StmtType::kTableDef);
  CreateFromStringTestBody<StmtType>(kST_COLUMN_DEF, StmtType::kColumnDef);
  CreateFromStringTestBody<StmtType>(kST_TABLE_CONSTRAINT, StmtType::kTableConstraint);
  CreateFromStringTestBody<StmtType>(kST_ALTER_ACTION_ADD, StmtType::kAlterActionAdd);
  CreateFromStringTestBody<StmtType>(kST_ALTER_ADD_LIST, StmtType::kAlterAddList);
  CreateFromStringTestBody<StmtType>(kST_ALTER_ACTION_DROP, StmtType::kAlterActionDrop);
  CreateFromStringTestBody<StmtType>(kST_ALTER_DROP_LIST, StmtType::kAlterDropList);
  CreateFromStringTestBody<StmtType>(kST_DROP_CONSTRAINT, StmtType::kDropConstraint);
  CreateFromStringTestBody<StmtType>(kST_DROP_COLUMN, StmtType::kDropColumn);

  CreateFromStringTestBody<StmtType>(kST_DELETE, StmtType::kDeleteStmt);
  CreateFromStringTestBody<StmtType>(kST_INSERT, StmtType::kInsertStmt);
  CreateFromStringTestBody<StmtType>(kST_UPDATE, StmtType::kUpdateStmt);

  CreateFromStringTestBody<StmtType>(kST_UPDATE_COLUMN, StmtType::kUpdateColumn);

  CreateFromStringTestBody<StmtType>(kST_CONDITION, StmtType::kCondition);
  CreateFromStringTestBody<StmtType>(kST_OR_CONDITION, StmtType::kORCondition);
  CreateFromStringTestBody<StmtType>(kST_AND_CONDITION, StmtType::kANDCondition);
  CreateFromStringTestBody<StmtType>(kST_NOT_CONDITION, StmtType::kNOTCondition);
  CreateFromStringTestBody<StmtType>(kST_PREDICATE, StmtType::kPredicate);
  CreateFromStringTestBody<StmtType>(kST_EXPRESSION, StmtType::kExpression);

  CreateFromStringTestBody<StmtType>(kST_MATH_EXPRESSION, StmtType::kMathExpression);
  CreateFromStringTestBody<StmtType>(kST_SUM, StmtType::kSum);
  CreateFromStringTestBody<StmtType>(kST_PRODUCT, StmtType::kProduct);
  CreateFromStringTestBody<StmtType>(kST_POWER, StmtType::kPower);
  CreateFromStringTestBody<StmtType>(kST_VALUE, StmtType::kValue);

  CreateFromStringTestBody<StmtType>(kST_OR_OPERATOR, StmtType::kOROperator);
  CreateFromStringTestBody<StmtType>(kST_AND_OPERATOR, StmtType::kANDOperator);
  CreateFromStringTestBody<StmtType>(kST_NOT_OPERATOR, StmtType::kNOTOperator);

  CreateFromStringTestBody<StmtType>(kST_PRIMARY_KEY, StmtType::kPrimaryKey);
  CreateFromStringTestBody<StmtType>(kST_FOREIGN_KEY, StmtType::kForeignKey);
  CreateFromStringTestBody<StmtType>(kST_NAME, StmtType::kName);
  CreateFromStringTestBody<StmtType>(kST_IDENTIFIER, StmtType::kIdentifier);
  CreateFromStringTestBody<StmtType>(kST_DOT_DELIMITER, StmtType::kDotDelimiter);
  CreateFromStringTestBody<StmtType>(kST_COMMA_DELIMITER, StmtType::kCommaDelimiter);
  CreateFromStringTestBody<StmtType>(kST_SEMICOLON_DELIMITER, StmtType::kSemicolonDelimiter);

  CreateFromStringTestBody<StmtType>(kST_CONSTRAINT_KW, StmtType::kConstraintKW);
  CreateFromStringTestBody<StmtType>(kST_COLUMN_KW, StmtType::kColumnKW);
  CreateFromStringTestBody<StmtType>(kST_REFERENCES_KW, StmtType::kReferencesKW);
  CreateFromStringTestBody<StmtType>(kST_ADD_KW, StmtType::kAddKW);
  CreateFromStringTestBody<StmtType>(kST_DROP_KW, StmtType::kDropKW);
  CreateFromStringTestBody<StmtType>(kST_INTO_KW, StmtType::kIntoKW);
  CreateFromStringTestBody<StmtType>(kST_SET_KW, StmtType::kSetKW);
  CreateFromStringTestBody<StmtType>(kST_FROM_KW, StmtType::kFromKW);
  CreateFromStringTestBody<StmtType>(kST_WHERE_KW, StmtType::kWhereKW);
  CreateFromStringTestBody<StmtType>(kST_VALUES_KW, StmtType::kValuesKW);

  CreateFromStringTestBody<StmtType>(kST_INT_TYPE, StmtType::kIntType);
  CreateFromStringTestBody<StmtType>(kST_INTEGER_TYPE, StmtType::kIntType);
  CreateFromStringTestBody<StmtType>(kST_FLOAT_TYPE, StmtType::kFloatType);
  CreateFromStringTestBody<StmtType>(kST_CHAR_TYPE, StmtType::kCharType);
  CreateFromStringTestBody<StmtType>(kST_VARCHAR_TYPE, StmtType::kVarcharType);

  CreateFromStringTestBody<StmtType>(kST_NULL_VALUE, StmtType::kNullValue);
  CreateFromStringTestBody<StmtType>(kST_UNKNOWN_VALUE, StmtType::kUnknownValue);
}

TEST(StmtTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<StmtType>("NOne", StmtType::kNone);
  CreateFromStringTestBody<StmtType>("DROP database", StmtType::kDropDatabaseStmt);
  CreateFromStringTestBody<StmtType>("alTER drOP List", StmtType::kAlterDropList);
  CreateFromStringTestBody<StmtType>("product", StmtType::kProduct);
  CreateFromStringTestBody<StmtType>("SEMICOLON DELIMITER", StmtType::kSemicolonDelimiter);
  CreateFromStringTestBody<StmtType>("SeT", StmtType::kSetKW);
  CreateFromStringTestBody<StmtType>("FloAT", StmtType::kFloatType);
}

TEST(StmtTypeStringTests, ToStringTest) {
  ToStringTestBody<StmtType>(StmtType::kNone, kST_NONE);
  ToStringTestBody<StmtType>(StmtType::kProgram, kST_PROGRAM);
  ToStringTestBody<StmtType>(StmtType::kQuery, kST_QUERY);
  ToStringTestBody<StmtType>(StmtType::kDdlStmt, kST_DDL_STMT);
  ToStringTestBody<StmtType>(StmtType::kDmlStmt, kST_DML_STMT);

  ToStringTestBody<StmtType>(StmtType::kAlterTableStmt, kST_ALTER_TABLE);
  ToStringTestBody<StmtType>(StmtType::kCreateDatabaseStmt, kST_CREATE_DATABASE);
  ToStringTestBody<StmtType>(StmtType::kCreateTableStmt, kST_CREATE_TABLE);
  ToStringTestBody<StmtType>(StmtType::kDropDatabaseStmt, kST_DROP_DATABASE);
  ToStringTestBody<StmtType>(StmtType::kDropTableStmt, kST_DROP_TABLE);

  ToStringTestBody<StmtType>(StmtType::kTableDef, kST_TABLE_DEF);
  ToStringTestBody<StmtType>(StmtType::kColumnDef, kST_COLUMN_DEF);
  ToStringTestBody<StmtType>(StmtType::kTableConstraint, kST_TABLE_CONSTRAINT);
  ToStringTestBody<StmtType>(StmtType::kAlterActionAdd, kST_ALTER_ACTION_ADD);
  ToStringTestBody<StmtType>(StmtType::kAlterAddList, kST_ALTER_ADD_LIST);
  ToStringTestBody<StmtType>(StmtType::kAlterActionDrop, kST_ALTER_ACTION_DROP);
  ToStringTestBody<StmtType>(StmtType::kAlterDropList, kST_ALTER_DROP_LIST);
  ToStringTestBody<StmtType>(StmtType::kDropConstraint, kST_DROP_CONSTRAINT);
  ToStringTestBody<StmtType>(StmtType::kDropColumn, kST_DROP_COLUMN);

  ToStringTestBody<StmtType>(StmtType::kDeleteStmt, kST_DELETE);
  ToStringTestBody<StmtType>(StmtType::kInsertStmt, kST_INSERT);
  ToStringTestBody<StmtType>(StmtType::kUpdateStmt, kST_UPDATE);

  ToStringTestBody<StmtType>(StmtType::kUpdateColumn, kST_UPDATE_COLUMN);

  ToStringTestBody<StmtType>(StmtType::kCondition, kST_CONDITION);
  ToStringTestBody<StmtType>(StmtType::kORCondition, kST_OR_CONDITION);
  ToStringTestBody<StmtType>(StmtType::kANDCondition, kST_AND_CONDITION);
  ToStringTestBody<StmtType>(StmtType::kNOTCondition, kST_NOT_CONDITION);
  ToStringTestBody<StmtType>(StmtType::kPredicate, kST_PREDICATE);
  ToStringTestBody<StmtType>(StmtType::kExpression, kST_EXPRESSION);

  ToStringTestBody<StmtType>(StmtType::kMathExpression, kST_MATH_EXPRESSION);
  ToStringTestBody<StmtType>(StmtType::kSum, kST_SUM);
  ToStringTestBody<StmtType>(StmtType::kProduct, kST_PRODUCT);
  ToStringTestBody<StmtType>(StmtType::kPower, kST_POWER);
  ToStringTestBody<StmtType>(StmtType::kValue, kST_VALUE);

  ToStringTestBody<StmtType>(StmtType::kOROperator, kST_OR_OPERATOR);
  ToStringTestBody<StmtType>(StmtType::kANDOperator, kST_AND_OPERATOR);
  ToStringTestBody<StmtType>(StmtType::kNOTOperator, kST_NOT_OPERATOR);

  ToStringTestBody<StmtType>(StmtType::kPrimaryKey, kST_PRIMARY_KEY);
  ToStringTestBody<StmtType>(StmtType::kForeignKey, kST_FOREIGN_KEY);
  ToStringTestBody<StmtType>(StmtType::kName, kST_NAME);
  ToStringTestBody<StmtType>(StmtType::kIdentifier, kST_IDENTIFIER);
  ToStringTestBody<StmtType>(StmtType::kDotDelimiter, kST_DOT_DELIMITER);
  ToStringTestBody<StmtType>(StmtType::kCommaDelimiter, kST_COMMA_DELIMITER);
  ToStringTestBody<StmtType>(StmtType::kSemicolonDelimiter, kST_SEMICOLON_DELIMITER);

  ToStringTestBody<StmtType>(StmtType::kConstraintKW, kST_CONSTRAINT_KW);
  ToStringTestBody<StmtType>(StmtType::kColumnKW, kST_COLUMN_KW);
  ToStringTestBody<StmtType>(StmtType::kReferencesKW, kST_REFERENCES_KW);
  ToStringTestBody<StmtType>(StmtType::kAddKW, kST_ADD_KW);
  ToStringTestBody<StmtType>(StmtType::kDropKW, kST_DROP_KW);
  ToStringTestBody<StmtType>(StmtType::kIntoKW, kST_INTO_KW);
  ToStringTestBody<StmtType>(StmtType::kSetKW, kST_SET_KW);
  ToStringTestBody<StmtType>(StmtType::kFromKW, kST_FROM_KW);
  ToStringTestBody<StmtType>(StmtType::kWhereKW, kST_WHERE_KW);
  ToStringTestBody<StmtType>(StmtType::kValuesKW, kST_VALUES_KW);

  ToStringTestBody<StmtType>(StmtType::kIntType, kST_INT_TYPE);
  ToStringTestBody<StmtType>(StmtType::kFloatType, kST_FLOAT_TYPE);
  ToStringTestBody<StmtType>(StmtType::kCharType, kST_CHAR_TYPE);
  ToStringTestBody<StmtType>(StmtType::kVarcharType, kST_VARCHAR_TYPE);

  ToStringTestBody<StmtType>(StmtType::kNullValue, kST_NULL_VALUE);
  ToStringTestBody<StmtType>(StmtType::kUnknownValue, kST_UNKNOWN_VALUE);
}

TEST(StmtTypeOperatorsTests, OutputTest) {
  OutputTestBody<StmtType, StmtType::Value>(StmtType::kNone, kST_NONE);
  OutputTestBody<StmtType, StmtType::Value>(StmtType::kColumnDef, kST_COLUMN_DEF);
  OutputTestBody<StmtType, StmtType::Value>(StmtType::kInsertStmt, kST_INSERT);
  OutputTestBody<StmtType, StmtType::Value>(StmtType::kPredicate, kST_PREDICATE);
  OutputTestBody<StmtType, StmtType::Value>(StmtType::kIdentifier, kST_IDENTIFIER);
}

TEST(StmtTypeOperatorsTests, CompareTwoEqualTypesTest) {
  StmtType first(StmtType::kQuery);
  StmtType second(StmtType::kQuery);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(StmtTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  StmtType first(StmtType::kColumnDef);
  StmtType second(StmtType::kUpdateStmt);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(StmtTypeOperatorsTests, CompareWithEqualValueTest) {
  StmtType type(StmtType::kPredicate);
  StmtType::Value value = StmtType::kPredicate;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(StmtTypeOperatorsTests, CompareWithDifferentValueTest) {
  StmtType type(StmtType::kDotDelimiter);
  StmtType::Value value = StmtType::kSemicolonDelimiter;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
