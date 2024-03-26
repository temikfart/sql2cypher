#include <sstream>
#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/ast/stmt_types.h"

using namespace scc::ast;
using namespace testing;

TEST(StmtTypeValueEnumTests, IsUnsignedCharTest) {
  int N_0 = 0;
  EXPECT_EQ((unsigned char) N_0, StmtType::Value(N_0));

  int N_neg1 = -1;
  EXPECT_EQ((unsigned char) N_neg1, StmtType::Value(N_neg1));

  int N_50 = 50;
  EXPECT_EQ((unsigned char) N_50, StmtType::Value(N_50));

  int N_128 = 128;
  EXPECT_EQ((unsigned char) N_128, StmtType::Value(N_128));

  int N_256 = 256;
  EXPECT_EQ((unsigned char) N_256, StmtType::Value(N_256));
}

TEST(StmtTypeCtorTests, DefaultValueTest) {
  StmtType type;
  EXPECT_EQ(StmtType::kNone, (StmtType::Value) type);
}

TEST(StmtTypeCtorTests, ValueTest) {
  EXPECT_EQ(StmtType::kNone, (StmtType::Value) StmtType(StmtType::kNone));
  EXPECT_EQ(StmtType::kProgram, (StmtType::Value) StmtType(StmtType::kProgram));
  EXPECT_EQ(StmtType::kAlterTableStmt, (StmtType::Value) StmtType(StmtType::kAlterTableStmt));
  EXPECT_EQ(StmtType::kReference, (StmtType::Value) StmtType(StmtType::kReference));
  EXPECT_EQ(StmtType::kVarcharType, (StmtType::Value) StmtType(StmtType::kVarcharType));
}

TEST(StmtTypeCtorTests, InvalidValueTest) {
  EXPECT_THROW(StmtType(StmtType::Value(-1)), std::invalid_argument);
  EXPECT_THROW(StmtType(StmtType::Value(100)), std::invalid_argument);
}

TEST(StmtTypeCtorTests, StringValueTest) {
  StmtType column_def(ST_COLUMN_DEF);
  EXPECT_EQ(StmtType::kColumnDef, (StmtType::Value) column_def);

  StmtType reference(ST_REFERENCE);
  EXPECT_EQ(StmtType::kReference, (StmtType::Value) reference);

  StmtType mixed_case_expression("expREssION");
  EXPECT_EQ(StmtType::kExpression, (StmtType::Value) mixed_case_expression);
}

TEST(StmtTypeCtorTests, InvalidStringValueTest) {
  EXPECT_THROW(StmtType("piece of wrong stmt"), std::invalid_argument);
  EXPECT_THROW(StmtType("lksdf7"), std::invalid_argument);
  EXPECT_THROW(StmtType("dr0p list"), std::invalid_argument);
  EXPECT_THROW(StmtType("non"), std::invalid_argument);
  EXPECT_THROW(StmtType("1nsert"), std::invalid_argument);
}

TEST(StmtTypeoStringTests, ToStringTest) {
  EXPECT_EQ(ST_NONE, StmtType(StmtType::kNone).ToString());
  EXPECT_EQ(ST_TABLE_DEF, StmtType(StmtType::kTableDef).ToString());
  EXPECT_EQ(ST_UPDATE, StmtType(StmtType::kUpdateStmt).ToString());
  EXPECT_EQ(ST_DOT_DELIMITER, StmtType(StmtType::kDotDelimiter).ToString());
  EXPECT_EQ(ST_VARCHAR_TYPE, StmtType(StmtType::kVarcharType).ToString());
}

TEST(StmtTypeCastTests, CastToValueTest) {
  StmtType alter_table_stmt(StmtType::kAlterTableStmt);
  auto alter_table_stmt_val = (StmtType::Value) alter_table_stmt;
  EXPECT_EQ(StmtType::kAlterTableStmt, alter_table_stmt_val);

  StmtType drop_table_stmt(StmtType::kDropTableStmt);
  auto drop_table_stmt_val = (StmtType::Value) drop_table_stmt;
  EXPECT_EQ(StmtType::kDropTableStmt, drop_table_stmt_val);
}

TEST(StmtTypeOperatorsTests, OutputTest) {
  std::ostringstream oss_create_database_stmt;
  oss_create_database_stmt << StmtType(StmtType::kCreateDatabaseStmt);
  EXPECT_EQ(ST_CREATE_DATABASE, oss_create_database_stmt.str());

  std::ostringstream oss_drop_list;
  oss_drop_list << StmtType(StmtType::kDropList);
  EXPECT_EQ(ST_DROP_LIST, oss_drop_list.str());

  std::ostringstream oss_condition;
  oss_condition << StmtType(StmtType::kCondition);
  EXPECT_EQ(ST_CONDITION, oss_condition.str());

  std::ostringstream oss_foreign_key;
  oss_foreign_key << StmtType(StmtType::kForeignKey);
  EXPECT_EQ(ST_FOREIGN_KEY, oss_foreign_key.str());
}

TEST(StmtTypeOperatorsTests, CompareTwoEqualTypesTest) {
  StmtType first(StmtType::kTableDef);
  StmtType second(StmtType::kTableDef);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(StmtTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  StmtType first(StmtType::kIdentifier);
  StmtType second(StmtType::kFloatType);

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
  StmtType type(StmtType::kPrimaryKey);
  StmtType::Value value = StmtType::kPrimaryKey;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(StmtTypeOperatorsTests, CompareWithDifferentValueTest) {
  StmtType type(StmtType::kCreateTableStmt);
  StmtType::Value value = StmtType::kColumnDef;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
