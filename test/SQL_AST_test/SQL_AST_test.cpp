#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/syntax_analyzer.h"
#include "SCC/tree_dump.h"

using namespace testing;

TEST(CreateTests, CreateDBTest) {
  // Creating database
}
TEST(CreateTests, CreateTableWithOneColumnTest) {
  // Creating table with one column
}
TEST(CreateTests, CreateTableWithColumnsOfAllTypesTest) {
  // Creating table with columns of all types
}
TEST(CreateTests, CreateTableWithSameColumnTypesTest) {
  // Creating table with same column types
}
TEST(CreateTests, CreateTableWithPKTest) {
  // Creating table with PRIMARY KEY
}
TEST(CreateTests, CreateTableWithMoreThanOnePKTest) {
  // Creating table with more than one PRIMARY KEY
}
TEST(CreateTests, CreateTableWithFKWhichRefsOnTableTest) {
  // Creating table with FOREIGN KEY which references on table
}
TEST(CreateTests, CreateTableWithFKWhichRefsOnColTest) {
  // Creating table with FOREIGN KEY which references on column
}
TEST(CreateTests, CreateTableWithNamedPKTest) {
  // Creating table with named PRIMARY KEY constraint
}
TEST(CreateTests, CreateTableWithNamedFKTest) {
  // Creating table with name FOREIGN KEY constraint
}
TEST(CreateTests, CreateTableWithDifferentConstraintsTest) {
  // Creating table with different constraints
}
TEST(AlterTests, AlterTableAddColumnTest) {
  // Alter table ADD column
}
TEST(AlterTests, AlterTableAddConstraintTest) {
  // Alter table ADD constraint
}
TEST(AlterTests, AlterTableAddColumnsAndConstraintsTest) {
  // Alter table ADD columns and constraints
}
TEST(AlterTests, AlterTableDropOneColumnTest) {
  // Alter table DROP one column
}
TEST(AlterTests, AlterTableDropMoreThanOneColumnTest) {
  // Alter table DROP more than one column
}
TEST(DropTests, DropTableOneTest) {
  // Drop one table
}
TEST(DropTests, DropTableMoreThanOneTest) {
  // Drop more than one table
}
TEST(DropTests, DropDatabaseOneTest) {
  // Drop one database
}
TEST(DropTests, DropDatabaseMoreThanOneTest) {
  // Drop more than one database
}
