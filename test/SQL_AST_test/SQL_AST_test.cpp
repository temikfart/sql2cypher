#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/lexical_analyzer.h"
#include "SCC/syntax_analyzer.h"
#include "SCC/tree_dump.h"
#include "logger/initializers/console_appender_initializer.hpp"

using namespace testing;

class CreateTests : public ::testing::Test {
protected:
  Tokenizer tTokenizer;
  SyntaxAnalyzer tSyntaxAnalyzer;

  const std::string tPath =  config.GetConfigPath() +
      "../resources/SQL_AST_test/positive_tests/";


  std::string png_dump_path_right_;
  std::string png_dump_path_test_;
  std::string dot_dump_path_test_;

  bool AreFilesEqual(std::string &path1, std::string &path2) {
    std::ifstream file1(path1, std::ifstream::in);
    std::ifstream file2(path2, std::ifstream::in);

    if (file1.fail() || file2.fail()) {
      return false;
    }

    file1.seekg(0, std::ifstream::end);
    file2.seekg(0, std::ifstream::end);
    if (file2.tellg() != file1.tellg()) {
      return false;
    }

    file1.seekg(0, std::ifstream::beg);
    file2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(file1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(file2.rdbuf()));
  }

  void SetUp() override {
    config.set_is_need_dump(true);
    logger::init(logger::Severity::silent, logger::StreamType::cout);
  }
  void TearDown() override {
    config.CloseInputFile();
    config.CloseOutputFile();
    config.CloseTreeDumpFile();

    std::string delete_cmd;
    delete_cmd = "rm \"" + png_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
    delete_cmd = "rm \"" + dot_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
  }
};
TEST_F(CreateTests, CreateDBTest) {
  png_dump_path_right_ = tPath + "create_db_test.png";
  png_dump_path_test_ = tPath + "create_db_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_db_test_tmp.txt";

  config.set_sql_path(tPath + "create_db_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithOneColumnTest) {
  png_dump_path_right_ = tPath + "create_table_with_one_column_test.png";
  png_dump_path_test_ = tPath + "create_table_with_one_column_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_one_column_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_one_column_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithColumnsOfAllTypesTest) {
  png_dump_path_right_ = tPath + "create_table_with_columns_of_all_types_test.png";
  png_dump_path_test_ = tPath + "create_table_with_columns_of_all_types_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_columns_of_all_types_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_columns_of_all_types_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithSameColumnTypesTest) {
  png_dump_path_right_ = tPath + "create_table_with_same_column_types_test.png";
  png_dump_path_test_ = tPath + "create_table_with_same_column_types_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_same_column_types_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_same_column_types_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithPKTest) {
  png_dump_path_right_ = tPath + "create_table_with_pk_test.png";
  png_dump_path_test_ = tPath + "create_table_with_pk_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_pk_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_pk_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithMoreThanOnePKTest) {
  png_dump_path_right_ = tPath + "create_table_with_more_than_one_pk_test.png";
  png_dump_path_test_ = tPath + "create_table_with_more_than_one_pk_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_more_than_one_pk_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_more_than_one_pk_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithFKWhichRefsOnTableTest) {
  png_dump_path_right_ = tPath + "create_table_with_fk_which_refs_on_table_test.png";
  png_dump_path_test_ = tPath + "create_table_with_fk_which_refs_on_table_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_fk_which_refs_on_table_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_fk_which_refs_on_table_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithFKWhichRefsOnColTest) {
  png_dump_path_right_ = tPath + "create_table_with_fk_which_refs_on_col_test.png";
  png_dump_path_test_ = tPath + "create_table_with_fk_which_refs_on_col_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_fk_which_refs_on_col_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_fk_which_refs_on_col_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithNamedPKTest) {
  png_dump_path_right_ = tPath + "create_table_with_named_pk_test.png";
  png_dump_path_test_ = tPath + "create_table_with_named_pk_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_named_pk_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_named_pk_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithNamedFKTest) {
  png_dump_path_right_ = tPath + "create_table_with_named_fk_test.png";
  png_dump_path_test_ = tPath + "create_table_with_named_fk_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_named_fk_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_named_fk_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(CreateTests, CreateTableWithDifferentConstraintsTest) {
  png_dump_path_right_ = tPath + "create_table_with_different_constraints_test.png";
  png_dump_path_test_ = tPath + "create_table_with_different_constraints_test_tmp.png";
  dot_dump_path_test_ = tPath + "create_table_with_different_constraints_test_tmp.txt";

  config.set_sql_path(tPath + "create_table_with_different_constraints_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}

class AlterTests : public ::testing::Test {
protected:
  Tokenizer tTokenizer;
  SyntaxAnalyzer tSyntaxAnalyzer;

  const std::string tPath =  config.GetConfigPath() +
      "../resources/SQL_AST_test/positive_tests/";


  std::string png_dump_path_right_;
  std::string png_dump_path_test_;
  std::string dot_dump_path_test_;

  bool AreFilesEqual(std::string &path1, std::string &path2) {
    std::ifstream file1(path1, std::ifstream::in);
    std::ifstream file2(path2, std::ifstream::in);

    if (file1.fail() || file2.fail()) {
      return false;
    }

    file1.seekg(0, std::ifstream::end);
    file2.seekg(0, std::ifstream::end);
    if (file2.tellg() != file1.tellg()) {
      return false;
    }

    file1.seekg(0, std::ifstream::beg);
    file2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(file1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(file2.rdbuf()));
  }

  void SetUp() override {
    config.set_is_need_dump(true);
  }
  void TearDown() override {
    config.CloseInputFile();
    config.CloseOutputFile();
    config.CloseTreeDumpFile();

    std::string delete_cmd;
    delete_cmd = "rm \"" + png_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
    delete_cmd = "rm \"" + dot_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
  }
};
TEST_F(AlterTests, AlterTableAddColumnTest) {
  png_dump_path_right_ = tPath + "alter_table_add_column_test.png";
  png_dump_path_test_ = tPath + "alter_table_add_column_test_tmp.png";
  dot_dump_path_test_ = tPath + "alter_table_add_column_test_tmp.txt";

  config.set_sql_path(tPath + "alter_table_add_column_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(AlterTests, AlterTableAddConstraintTest) {
  png_dump_path_right_ = tPath + "alter_table_add_constraint_test.png";
  png_dump_path_test_ = tPath + "alter_table_add_constraint_test_tmp.png";
  dot_dump_path_test_ = tPath + "alter_table_add_constraint_test_tmp.txt";

  config.set_sql_path(tPath + "alter_table_add_constraint_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(AlterTests, AlterTableAddColumnsAndConstraintsTest) {
  png_dump_path_right_ = tPath + "alter_table_add_column_and_constraint_test.png";
  png_dump_path_test_ = tPath + "alter_table_add_column_and_constraint_test_tmp.png";
  dot_dump_path_test_ = tPath + "alter_table_add_column_and_constraint_test_tmp.txt";

  config.set_sql_path(tPath + "alter_table_add_column_and_constraint_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(AlterTests, AlterTableDropOneColumnTest) {
  png_dump_path_right_ = tPath + "alter_table_drop_one_column_test.png";
  png_dump_path_test_ = tPath + "alter_table_drop_one_column_test_tmp.png";
  dot_dump_path_test_ = tPath + "alter_table_drop_one_column_test_tmp.txt";

  config.set_sql_path(tPath + "alter_table_drop_one_column_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(AlterTests, AlterTableDropMoreThanOneColumnTest) {
  png_dump_path_right_ = tPath + "alter_table_drop_more_than_one_column_test.png";
  png_dump_path_test_ = tPath + "alter_table_drop_more_than_one_column_test_tmp.png";
  dot_dump_path_test_ = tPath + "alter_table_drop_more_than_one_column_test_tmp.txt";

  config.set_sql_path(tPath + "alter_table_drop_more_than_one_column_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}

class DropTests : public ::testing::Test {
protected:
  Tokenizer tTokenizer;
  SyntaxAnalyzer tSyntaxAnalyzer;

  const std::string tPath =  config.GetConfigPath() +
      "../resources/SQL_AST_test/positive_tests/";


  std::string png_dump_path_right_;
  std::string png_dump_path_test_;
  std::string dot_dump_path_test_;

  bool AreFilesEqual(std::string &path1, std::string &path2) {
    std::ifstream file1(path1, std::ifstream::in);
    std::ifstream file2(path2, std::ifstream::in);

    if (file1.fail() || file2.fail()) {
      return false;
    }

    file1.seekg(0, std::ifstream::end);
    file2.seekg(0, std::ifstream::end);
    if (file2.tellg() != file1.tellg()) {
      return false;
    }

    file1.seekg(0, std::ifstream::beg);
    file2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(file1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(file2.rdbuf()));
  }

  void SetUp() override {
    config.set_is_need_dump(true);
  }
  void TearDown() override {
    config.CloseInputFile();
    config.CloseOutputFile();
    config.CloseTreeDumpFile();

    std::string delete_cmd;
    delete_cmd = "rm \"" + png_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
    delete_cmd = "rm \"" + dot_dump_path_test_ + "\"";
    std::system(delete_cmd.c_str());
  }
};
TEST_F(DropTests, DropTableOneTest) {
  png_dump_path_right_ = tPath + "drop_table_one_test.png";
  png_dump_path_test_ = tPath + "drop_table_one_test_tmp.png";
  dot_dump_path_test_ = tPath + "drop_table_one_test_tmp.txt";

  config.set_sql_path(tPath + "drop_table_one_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(DropTests, DropTableMoreThanOneTest) {
  png_dump_path_right_ = tPath + "drop_table_more_than_one_test.png";
  png_dump_path_test_ = tPath + "drop_table_more_than_one_test_tmp.png";
  dot_dump_path_test_ = tPath + "drop_table_more_than_one_test_tmp.txt";

  config.set_sql_path(tPath + "drop_table_more_than_one_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(DropTests, DropDatabaseOneTest) {
  png_dump_path_right_ = tPath + "drop_database_one_test.png";
  png_dump_path_test_ = tPath + "drop_database_one_test_tmp.png";
  dot_dump_path_test_ = tPath + "drop_database_one_test_tmp.txt";

  config.set_sql_path(tPath + "drop_database_one_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
TEST_F(DropTests, DropDatabaseMoreThanOneTest) {
  png_dump_path_right_ = tPath + "drop_database_more_than_one_test.png";
  png_dump_path_test_ = tPath + "drop_database_more_than_one_test_tmp.png";
  dot_dump_path_test_ = tPath + "drop_database_more_than_one_test_tmp.txt";

  config.set_sql_path(tPath + "drop_database_more_than_one_test.sql");
  config.set_tree_dump_path(dot_dump_path_test_);
  config.Start(1, nullptr);

  TreeDump tDump;

  tTokenizer.Tokenize();

  std::shared_ptr<Node> tAST =
      tSyntaxAnalyzer.Analyze(tTokenizer.get_tokens_array());

  tDump.DumpTree(tAST);

  EXPECT_TRUE(AreFilesEqual(png_dump_path_test_, png_dump_path_right_));
}
