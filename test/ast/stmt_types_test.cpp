#include "gtest/gtest.h"

#include "SCC/ast/stmt_types.h"

//using namespace scc;
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
  EXPECT_EQ(StmtType::EMPTY_TYPE, (StmtType::Value) type);
}

TEST(StmtTypeCtorTests, ValueTest) {
  EXPECT_EQ(StmtType::EMPTY_TYPE, (StmtType::Value) StmtType(StmtType::EMPTY_TYPE));
  EXPECT_EQ(StmtType::Program, (StmtType::Value) StmtType(StmtType::Program));
  EXPECT_EQ(StmtType::alterTableStatement, (StmtType::Value) StmtType(StmtType::alterTableStatement));
  EXPECT_EQ(StmtType::reference, (StmtType::Value) StmtType(StmtType::reference));
  EXPECT_EQ(StmtType::SQL_varchar, (StmtType::Value) StmtType(StmtType::SQL_varchar));
}

TEST(StmtTypeCtorTests, InvalidValueTest) {
  EXPECT_THROW(StmtType(StmtType::Value(-1)), std::invalid_argument);
  EXPECT_THROW(StmtType(StmtType::Value(100)), std::invalid_argument);
}

TEST(StmtTypeCastTests, CastToValueTest) {
  StmtType alter_table_stmt(StmtType::alterTableStatement);
  auto alter_table_stmt_val = (StmtType::Value) alter_table_stmt;
  EXPECT_EQ(StmtType::alterTableStatement, alter_table_stmt_val);

  StmtType drop_table_stmt(StmtType::dropTableStatement);
  auto drop_table_stmt_val = (StmtType::Value) drop_table_stmt;
  EXPECT_EQ(StmtType::dropTableStatement, drop_table_stmt_val);
}

TEST(StmtTypeOperatorsTests, CompareTwoEqualTypesTest) {
  StmtType first(StmtType::tableDefinition);
  StmtType second(StmtType::tableDefinition);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(StmtTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  StmtType first(StmtType::identifier);
  StmtType second(StmtType::SQL_float);

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
  StmtType type(StmtType::primaryKey);
  StmtType::Value value = StmtType::primaryKey;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(StmtTypeOperatorsTests, CompareWithDifferentValueTest) {
  StmtType type(StmtType::createTableStatement);
  StmtType::Value value = StmtType::columnDefinition;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
